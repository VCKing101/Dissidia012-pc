// Room for models bigger than the game's own; see d012_model_memory.hpp. MIT
// licence; see port/README.md.
//
// The game's side (USA main; EU code is the same):
// - main 0x088907C4 builds the file heap: 0x0888204C(0x275800) takes its
//   block from the main heap and 0x0885C40C(block, 0x275800, name, 0x10)
//   makes a heap of it, whose handle is the block's address; 0x089110B4 gives
//   the heap model slots are taken from (*(*0x089D83A0 + 4)).
// - main 0x088667DC builds the battle model pool the same way, 0x780000 bytes
//   (from 0x08866718, at game start).
// - Model slots are registered with 0x0884417C(manager, id, heap, 0x264000,
//   0) (battle 0x08B45310, main 0x08844C14, 0x089121F4, 0x08922B90), which
//   hands them to 0x08843014: that takes the slot's whole size from the heap
//   (0x0885C6CC), loads the file into it and frees it (0x0885C538).
// - main 0x088A0C10 builds the sound heap: 0x0888204C(0x237000) takes its
//   block from the main heap and 0x08867310(0x09B4CF80, block, 0x237000,
//   0x40) makes a handle heap of it at 0x09B4CF80; 0x088A0C6C gives it back
//   (0x0888208C). 0x088A0CB0(size) takes from it; a fighter's two banks
//   (moveset chunk kind 14) are copied in by 0x08842774.
// - main 0x088A5054 builds the effect heap: a kernel memory block of
//   0x3DD000 bytes (sceKernelAllocMemoryBlock) made a handle heap at
//   0x09B4D420 the same way; 0x088A51C0(size) takes from it, and a fighter's
//   effect package (chunk `effe`) is copied in by 0x0883EB10. Each heap
//   object is one global, so there is never more than one of each.
// - 0x088673CC(heap, size) takes from a handle heap, 0x0885C6CC(heap, size,
//   alignment, ...) from a block heap; both return 0 when there is no room.
// The hooks sit on the callees, which are in other generated units than
// their callers, so every call reaches them through dispatch (registering a
// function takes its unit off PortableKit's direct-chaining path); calls
// from within a callee's own unit would not, and none of these comes from
// one. With no model bigger than the game's, nothing is hooked.

#include "d012_model_memory.hpp"

#include "hle/hle_common.hpp"
#include "kernel/kernel.hpp"
#include "profile.hpp"

#include "psprecomp/runtime.hpp"

#include <algorithm>
#include <cstdio>
#include <initializer_list>
#include <iostream>
#include <string>

namespace d012::model_memory {
namespace {

using portablekit::kernel;
using psprecomp::AllegrexContext;
using psprecomp::CorpusRuntime;
using psprecomp::Runtime;

constexpr std::uint32_t kReserveBase = 0x0A000000u;  // the end of the game's 32 MiB

constexpr std::uint32_t kAlloc = 0x0888204Cu;     // main heap: alloc(size)
constexpr std::uint32_t kFree = 0x0888208Cu;      // main heap: free(block)
constexpr std::uint32_t kMakeHeap = 0x0885C40Cu;  // make_heap(block, size, name, alignment)
constexpr std::uint32_t kSlotLoad = 0x08843014u;  // a model slot: (slot, id, heap, size, flag)
// The return addresses of the two alloc calls, in 0x088907C4 and 0x088667DC.
constexpr std::uint32_t kFileHeapCall = 0x088907E8u;
constexpr std::uint32_t kPoolCall = 0x088667FCu;
// The sound heap: its alloc call's return address in 0x088A0C10, the handle
// heap's make (heap, block, size, alignment), the heap object, and its
// alloc(size) and lock(handle).
constexpr std::uint32_t kSoundHeapCall = 0x088A0C24u;
constexpr std::uint32_t kHandleHeapMake = 0x08867310u;
constexpr std::uint32_t kSoundHeap = 0x09B4CF80u;
constexpr std::uint32_t kEffectHeap = 0x09B4D420u;
constexpr std::uint32_t kRetailEffectHeap = 0x3DD000u;
// The allocators D012_TRACE_HEAPS watches.
constexpr std::uint32_t kHandleHeapAlloc = 0x088673CCu;
constexpr std::uint32_t kBlockHeapAlloc = 0x0885C6CCu;

// The file heap keeps the game's own room beside the block (0x11800 bytes)
// and this much more. The pool's growth is an estimate: the models built in
// it come from the files, so it grows by the block's growth for each of two
// fighters, and by this much besides.
constexpr std::uint32_t kFileHeapMargin = 0x10000u;
constexpr std::uint32_t kPoolMargin = 0x80000u;
constexpr std::uint32_t kAlign = 0x10000u;

std::uint64_t align_up(std::uint64_t value, std::uint64_t to) { return (value + to - 1u) / to * to; }

std::string hex(std::uint32_t value) {
    char text[11];
    std::snprintf(text, sizeof text, "0x%08X", value);
    return text;
}

struct Block {
    std::uint32_t address{};
    std::uint32_t size{};
    bool in_use{};
    const char *name{};
};

struct State {
    std::uint32_t base{}, end{};  // the reserved memory
    std::uint32_t slot{};         // the staging block's size; 0: the game's own
    Block file{}, pool{}, sound{}, effects{};
    Runtime::RecompiledFunction alloc{}, free{}, make_heap{}, slot_load{};
    Runtime::RecompiledFunction handle_heap_make{}, handle_heap_alloc{}, block_heap_alloc{};
    bool main_heap_hooked{};  // alloc and free, which the models and the sounds share
    bool handle_heap_hooked{};
    bool trace{};
    int trace_heaps{};        // D012_TRACE_HEAPS: 1 what does not fit, 2 every ask
    bool said_unfinished{};
    bool said_slot{}, said_other_heap{};
};

State &state() {
    static State s;
    return s;
}

void trace(const std::string &line) {
    if (state().trace) std::cout << "[mods] model memory: " << line << std::endl;
}

// Returns from a hooked function as its own `jr $ra` would.
void leave(AllegrexContext &ctx) { ctx.pc = ctx.gpr[31]; }

void on_alloc(CorpusRuntime &rt, AllegrexContext &ctx) {
    State &s = state();
    const std::uint32_t size = ctx.gpr[4], from = ctx.gpr[31];
    Block *block = from == kFileHeapCall && size == kRetailFileHeap    ? &s.file
                   : from == kPoolCall && size == kRetailPool          ? &s.pool
                   : from == kSoundHeapCall && size == kRetailSoundHeap ? &s.sound
                                                                        : nullptr;
    if (block != nullptr && block->size == 0u) block = nullptr;  // not grown
    if (block != nullptr && !block->in_use) {
        block->in_use = true;
        trace(std::string(block->name) + " " + std::to_string(block->size) + " bytes at " + hex(block->address));
        ctx.gpr[2] = block->address;
        leave(ctx);
        return;
    }
    if (block != nullptr)
        std::cerr << "[mods] warning: the game asked for a second " << block->name
                  << " while the first is in use; it gets the game's own size\n";
    s.alloc(rt, ctx);
}

void on_free(CorpusRuntime &rt, AllegrexContext &ctx) {
    State &s = state();
    for (Block *block : {&s.file, &s.pool, &s.sound}) {
        if (block->size == 0u || ctx.gpr[4] != block->address || !block->in_use) continue;
        // Kept for the next battle's, as the main heap would give it back.
        block->in_use = false;
        trace(std::string(block->name) + " given back");
        ctx.gpr[2] = 0u;
        leave(ctx);
        return;
    }
    s.free(rt, ctx);
}

void on_make_heap(CorpusRuntime &rt, AllegrexContext &ctx) {
    State &s = state();
    if (ctx.gpr[4] == s.file.address && ctx.gpr[5] == kRetailFileHeap) ctx.gpr[5] = s.file.size;
    else if (ctx.gpr[4] == s.pool.address && ctx.gpr[5] == kRetailPool) ctx.gpr[5] = s.pool.size;
    s.make_heap(rt, ctx);
}

void on_handle_heap_make(CorpusRuntime &rt, AllegrexContext &ctx) {
    State &s = state();
    if (s.trace_heaps > 0 && (ctx.gpr[4] == kSoundHeap || ctx.gpr[4] == kEffectHeap))
        std::cout << "[heaps] making the " << (ctx.gpr[4] == kSoundHeap ? "sound" : "effect") << " heap: the kernel has "
                  << kernel().free_memory() << " bytes free below the reserved memory, at most "
                  << kernel().largest_free_memory() << " in one block" << std::endl;
    if (s.sound.size != 0u && ctx.gpr[4] == kSoundHeap && ctx.gpr[5] == s.sound.address && ctx.gpr[6] == kRetailSoundHeap) {
        ctx.gpr[6] = s.sound.size;
    } else if (s.effects.size != 0u && ctx.gpr[4] == kEffectHeap && ctx.gpr[6] == kRetailEffectHeap) {
        // The game's own block stays taken, unused, as the game would hold it.
        ctx.gpr[5] = s.effects.address;
        ctx.gpr[6] = s.effects.size;
        trace("effect heap " + std::to_string(s.effects.size) + " bytes at " + hex(s.effects.address));
    }
    s.handle_heap_make(rt, ctx);
}

// D012_TRACE_HEAPS: an allocation the heap had no room for (=2: every one).
void say_alloc(const char *kind, std::uint32_t heap, std::uint32_t size, std::uint32_t from, const AllegrexContext &ctx) {
    State &s = state();
    if (ctx.pc != from) {
        if (!s.said_unfinished) std::cout << "[heaps] an allocation did not finish in its call; not traced" << std::endl;
        s.said_unfinished = true;
        return;
    }
    if (ctx.gpr[2] == 0u)
        std::cout << "[heaps] NO ROOM: " << kind << " heap " << hex(heap) << ", " << size << " bytes (from " << hex(from)
                  << ")" << std::endl;
    else if (s.trace_heaps > 1)
        std::cout << "[heaps] " << kind << " heap " << hex(heap) << ", " << size << " bytes (from " << hex(from) << ")"
                  << std::endl;
}
void on_handle_heap_alloc(CorpusRuntime &rt, AllegrexContext &ctx) {
    State &s = state();
    const std::uint32_t heap = ctx.gpr[4], size = ctx.gpr[5], from = ctx.gpr[31];
    s.handle_heap_alloc(rt, ctx);
    say_alloc("handle", heap, size, from, ctx);
}
void on_block_heap_alloc(CorpusRuntime &rt, AllegrexContext &ctx) {
    State &s = state();
    const std::uint32_t heap = ctx.gpr[4], size = ctx.gpr[5], from = ctx.gpr[31];
    s.block_heap_alloc(rt, ctx);
    say_alloc("block", heap, size, from, ctx);
}

void on_slot_load(CorpusRuntime &rt, AllegrexContext &ctx) {
    State &s = state();
    if (ctx.gpr[7] == kRetailBlock) {
        // Only a slot taken from the grown file heap may be bigger: a heap
        // made in its block (the heap's handle is where its block starts).
        const std::uint32_t heap = ctx.gpr[6] & 0x1FFFFFFFu;
        const bool grown = s.file.in_use && heap >= s.file.address && heap < s.file.address + s.file.size;
        if (grown) {
            ctx.gpr[7] = s.slot;
            if (!s.said_slot || s.trace) {
                s.said_slot = true;
                std::cout << "[mods] model memory: a model slot of " << s.slot << " bytes (id " << ctx.gpr[5]
                          << ")" << std::endl;
            }
        } else if (!s.said_other_heap) {
            s.said_other_heap = true;
            std::cerr << "[mods] warning: a model slot (id " << ctx.gpr[5] << ") is taken from a heap other than "
                      << "the file heap; it keeps the game's size, " << kRetailBlock << " bytes\n";
        }
    }
    s.slot_load(rt, ctx);
}

struct Hook {
    std::uint32_t address;
    Runtime::RecompiledFunction *original;
    Runtime::RecompiledFunction function;
    const char *name;
};

// All of them or none.
bool hook_all(Runtime &rt, std::initializer_list<Hook> hooks) {
    for (const Hook &h : hooks) {
        *h.original = rt.registered_function(h.address);
        if (*h.original != nullptr) continue;
        std::cerr << "[mods] error: no recompiled function at " << hex(h.address)
                  << " to hook; models keep the game's sizes\n";
        return false;
    }
    for (const Hook &h : hooks) rt.register_function(h.address, h.function, std::string("d012_model_memory_") + h.name);
    return true;
}

// The main heap's alloc and free, once for both the models and the sounds.
bool hook_main_heap(Runtime &rt) {
    State &s = state();
    if (!s.main_heap_hooked)
        s.main_heap_hooked = hook_all(rt, {{kAlloc, &s.alloc, &on_alloc, "alloc"}, {kFree, &s.free, &on_free, "free"}});
    return s.main_heap_hooked;
}

} // namespace

void reserve() {
    State &s = state();
    if (s.base != 0u) return;
    Runtime &rt = kernel().runtime();
    const std::uint32_t end = psprecomp::GuestMemory::kPhysicalBase + rt.memory().size();
    if (end <= kReserveBase) return;
    const std::int32_t uid = kernel().allocate_block("d012 port memory", 2u, end - kReserveBase, kReserveBase);
    const portablekit::MemoryBlock *block = uid > 0 ? kernel().find_block(uid) : nullptr;
    if (block == nullptr) {
        std::cerr << "[mods] warning: guest memory above " << hex(kReserveBase)
                  << " could not be reserved; bigger models cannot be given room\n";
        return;
    }
    s.base = block->address;
    s.end = block->address + block->size;
}

std::uint64_t block_size() { return state().slot != 0u ? state().slot : kRetailBlock; }

namespace {

// The largest model file the reserved memory holds room for.
std::uint64_t largest_possible() {
    const State &s = state();
    if (s.base == 0u) return kRetailBlock;
    // file heap + pool = retail sizes + 3 * growth + margins, each aligned.
    const std::uint64_t fixed = align_up(kRetailFileHeap + kFileHeapMargin, kAlign) +
                                align_up(kRetailPool + kPoolMargin, kAlign) + 2u * kAlign;
    const std::uint64_t room = s.end - s.base;
    if (room <= fixed) return kRetailBlock;
    return kRetailBlock + (room - fixed) / 3u / 0x1000u * 0x1000u;
}

} // namespace

void fit(std::uint64_t largest) {
    State &s = state();
    if (largest <= kRetailBlock || s.slot != 0u) return;
    if (portablekit::env("NO_MODEL_MEMORY") != nullptr) {
        std::cout << "[mods] model memory off (" << portablekit::env_name("NO_MODEL_MEMORY")
                  << "): models keep the game's sizes" << std::endl;
        return;
    }
    if (s.base == 0u) return;  // reserve() said why
    s.trace = portablekit::env("TRACE_MODEL_MEMORY") != nullptr;
    const std::uint64_t slot = align_up(std::min(largest, largest_possible()), 0x1000u);
    const std::uint64_t growth = slot - kRetailBlock;
    s.slot = static_cast<std::uint32_t>(slot);
    s.file = {s.base, static_cast<std::uint32_t>(kRetailFileHeap + growth + kFileHeapMargin), false, "file heap"};
    s.pool = {static_cast<std::uint32_t>(align_up(s.file.address + s.file.size, kAlign)),
              static_cast<std::uint32_t>(kRetailPool + 2u * growth + kPoolMargin), false, "battle model pool"};

    Runtime &rt = kernel().runtime();
    if (!hook_main_heap(rt) || !hook_all(rt, {{kMakeHeap, &s.make_heap, &on_make_heap, "make_heap"},
                                              {kSlotLoad, &s.slot_load, &on_slot_load, "slot_load"}})) {
        s.slot = 0u;
        s.file = s.pool = Block{};
        return;
    }
    std::cout << "[mods] model memory: model files up to " << s.slot << " bytes (the game's " << kRetailBlock
              << "); file heap " << s.file.size << " bytes at " << hex(s.file.address) << ", battle model pool "
              << s.pool.size << " bytes at " << hex(s.pool.address) << std::endl;
}

void trace_heaps() {
    State &s = state();
    const char *level = portablekit::env("TRACE_HEAPS");
    if (s.trace_heaps != 0 || level == nullptr) return;
    s.trace_heaps = std::string(level) == "2" ? 2 : 1;
    if (!s.handle_heap_hooked)
        s.handle_heap_hooked = hook_all(kernel().runtime(), {{kHandleHeapMake, &s.handle_heap_make, &on_handle_heap_make,
                                                              "handle_heap_make"}});
    if (!hook_all(kernel().runtime(), {{kHandleHeapAlloc, &s.handle_heap_alloc, &on_handle_heap_alloc, "handle_heap_alloc"},
                                       {kBlockHeapAlloc, &s.block_heap_alloc, &on_block_heap_alloc, "block_heap_alloc"}}))
        s.trace_heaps = -1;
}

void fit_battle_heaps(std::uint32_t sound_extra, std::uint32_t effect_extra) {
    State &s = state();
    if ((sound_extra == 0u && effect_extra == 0u) || s.sound.size != 0u || s.effects.size != 0u) return;
    if (portablekit::env("NO_MODEL_MEMORY") != nullptr) {
        std::cout << "[mods] model memory off (" << portablekit::env_name("NO_MODEL_MEMORY")
                  << "): the sound and effect heaps keep the game's sizes" << std::endl;
        return;
    }
    if (s.base == 0u) return;  // reserve() said why
    if (s.slot != 0u) {
        std::cerr << "[mods] warning: the battle heaps are set after the models' blocks; they keep the game's sizes\n";
        return;
    }
    s.trace = s.trace || portablekit::env("TRACE_MODEL_MEMORY") != nullptr;
    // From the top of the reserved memory, so fit() lays the models' blocks
    // out from its base as before.
    std::uint32_t top = s.end;
    const auto take = [&](std::uint32_t retail, std::uint32_t extra, const char *name) {
        const std::uint32_t size = static_cast<std::uint32_t>(align_up(std::uint64_t(retail) + extra, kAlign));
        if (extra == 0u || top - s.base < size + kAlign) return Block{};
        top = (top - size) / kAlign * kAlign;
        return Block{top, size, false, name};
    };
    s.sound = take(kRetailSoundHeap, sound_extra, "sound heap");
    s.effects = take(kRetailEffectHeap, effect_extra, "effect heap");
    Runtime &rt = kernel().runtime();
    const bool hooked = (s.sound.size == 0u || hook_main_heap(rt)) &&
                        (s.handle_heap_hooked ||
                         (s.handle_heap_hooked = hook_all(rt, {{kHandleHeapMake, &s.handle_heap_make, &on_handle_heap_make,
                                                                 "handle_heap_make"}})));
    if (!hooked) {
        s.sound = s.effects = Block{};
        return;
    }
    s.end = top;
    for (const Block *block : {&s.sound, &s.effects})
        if (block->size != 0u)
            std::cout << "[mods] model memory: " << block->name << " " << block->size << " bytes at " << hex(block->address)
                      << " (the game's " << (block == &s.sound ? kRetailSoundHeap : kRetailEffectHeap) << ")" << std::endl;
}

} // namespace d012::model_memory
