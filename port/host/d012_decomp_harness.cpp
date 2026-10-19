// Decompiled functions as native overrides, and their differential test
// against the recompiled originals. See d012_decomp_harness.hpp. MIT licence;
// see port/README.md.

#include "d012_decomp_harness.hpp"

#include "app_paths.hpp"
#include "kernel/kernel.hpp"
#include "overlay_module.hpp"
#include "overlays.hpp"
#include "profile.hpp"

#include "psprecomp/common.hpp"
#include "psprecomp/corpus_abi.hpp"
#include "psprecomp/elf32.hpp"
#include "psprecomp/runtime.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace d012::decomp {
namespace {

using psprecomp::AllegrexContext;
using psprecomp::CorpusRuntime;
using psprecomp::Runtime;

// Nothing is registered here; a guest call made by decompiled code returns
// to it, which ends the nested run.
constexpr std::uint32_t kReturnMagic = 0x08800100u;
// Bytes below the caller's $sp that are dead after the call and not compared.
constexpr std::uint32_t kDeadStack = 0x10000u;
// The overlay window.
constexpr std::uint32_t kWindowBase = 0x089DD4C0u;
constexpr std::uint32_t kWindowEnd = 0x08B62F70u;

// Cross-unit chaining runs the kernel's preemption at chain boundaries. A
// nested guest call must not switch threads under its host caller, so it
// runs with chaining off; the limit is a protected member of the corpus
// runtime, reached through a derived class.
struct ChainLimit : CorpusRuntime {
    static std::uint32_t CorpusRuntime::*member() { return &ChainLimit::chain_depth_limit_; }
};

struct ChainOff {
    Runtime &rt;
    std::uint32_t saved;
    explicit ChainOff(Runtime &runtime) : rt(runtime), saved(runtime.*ChainLimit::member()) {
        rt.*ChainLimit::member() = 0u;
    }
    ~ChainOff() { rt.*ChainLimit::member() = saved; }
    ChainOff(const ChainOff &) = delete;
    ChainOff &operator=(const ChainOff &) = delete;
};

std::string hex(std::uint32_t value) { return psprecomp::hex32(value); }

[[noreturn]] void fatal(const std::string &message) {
    std::cerr << "[decomp] FATAL: " << message << std::endl;
    std::fflush(nullptr);
    std::abort();
}

CallState state_of(const AllegrexContext &ctx) {
    CallState s;
    s.gpr = ctx.gpr;
    s.fpr = ctx.fpr;
    return s;
}

void load_arguments(AllegrexContext &c, const GuestRegs &r) {
    for (int i = 0; i < 4; ++i) c.gpr[4 + i] = r.a[i];
    for (int i = 0; i < 4; ++i) c.gpr[8 + i] = r.a[4 + i];
    for (int i = 0; i < 4; ++i) c.fpr[12 + i] = r.f[i];
}

GuestRegs arguments_of(const CallState &s) {
    GuestRegs r{};
    for (int i = 0; i < 4; ++i) r.a[i] = s.gpr[4 + i];
    for (int i = 0; i < 4; ++i) r.a[4 + i] = s.gpr[8 + i];
    for (int i = 0; i < 4; ++i) r.f[i] = s.fpr[12 + i];
    return r;
}

// ---- Overlay libraries ------------------------------------------------------

struct Library {
    std::string name;
    std::uint32_t base{}, size{}, code_size{};
    std::uint64_t hash{};
    decltype(&portablekit_register_overlay) install{};
};

std::vector<Library> &libraries() {
    static std::vector<Library> list;
    static bool loaded = false;
    if (loaded) return list;
    loaded = true;
    std::filesystem::path directory = portablekit::bundled_overlay_directory();
    if (const char *dir = portablekit::env("OVERLAY_DIR"); dir != nullptr && *dir != '\0') directory = dir;
    std::error_code ec;
    for (const auto &item : std::filesystem::directory_iterator(directory, ec)) {
        const auto &path = item.path();
#if defined(_WIN32)
        if (path.extension() != ".dll") continue;
        HMODULE handle = LoadLibraryW(path.wstring().c_str());  // already loaded by the host: same module
        if (handle == nullptr) continue;
        auto info_of = reinterpret_cast<decltype(&portablekit_overlay_info)>(
            reinterpret_cast<void *>(GetProcAddress(handle, "portablekit_overlay_info")));
        auto install = reinterpret_cast<decltype(&portablekit_register_overlay)>(
            reinterpret_cast<void *>(GetProcAddress(handle, "portablekit_register_overlay")));
#else
        if (path.extension() != ".so" && path.extension() != ".dylib") continue;
        void *handle = dlopen(path.string().c_str(), RTLD_NOW);
        if (handle == nullptr) continue;
        auto info_of = reinterpret_cast<decltype(&portablekit_overlay_info)>(dlsym(handle, "portablekit_overlay_info"));
        auto install = reinterpret_cast<decltype(&portablekit_register_overlay)>(
            dlsym(handle, "portablekit_register_overlay"));
#endif
        if (info_of == nullptr || install == nullptr) continue;
        const portablekit::OverlayModuleInfo *info = info_of();
        list.push_back(Library{info->name, info->base, info->size, info->code_size, info->hash, install});
    }
    return list;
}

std::uint64_t fnv1a64(const std::uint8_t *data, std::size_t size) {
    std::uint64_t hash = 0xCBF29CE484222325ull;
    for (std::size_t i = 0; i < size; ++i) {
        hash ^= data[i];
        hash *= 0x100000001B3ull;
    }
    return hash;
}

// The library whose image is in the overlay window now, or null.
const Library *resident_overlay(const psprecomp::GuestMemory &memory) {
    for (const Library &library : libraries()) {
        if (library.base != kWindowBase) continue;
        const std::size_t length = portablekit::kOverlayHeaderBytes + library.code_size;
        const std::uint8_t *image = memory.raw_pointer(library.base, length);
        if (image != nullptr && fnv1a64(image, length) == library.hash) return &library;
    }
    return nullptr;
}

// ---- State -----------------------------------------------------------------

struct Registered {
    Override o;
    std::uint64_t calls = 0;
    std::uint64_t verified = 0;
};

struct Harness {
    bool enabled = false;      // overrides run in the game
    bool verify = false;       // the differential test
    bool verify_only = false;  // test, but leave the originals in the game
    std::uint64_t verify_limit = 25;
    std::vector<Registered> overrides;
    std::map<std::uint32_t, std::size_t> by_address;
    bool installed_main = false;
    bool rearm_pending = false;
    const Library *armed_overlay = nullptr;
    std::uint64_t dispatches = 0;
    std::map<std::string, std::uint64_t> probes_done;
    std::uint64_t same = 0, different = 0, inconclusive = 0;
    std::map<std::uint32_t, ReferenceStub> stubs;
    Runtime *live = nullptr;  // the game's runtime, once it has run
};

Harness &harness() {
    static Harness h;
    return h;
}

// ---- Running guest code from a host function -------------------------------

struct Machine {
    Runtime &rt;
    const AllegrexContext &caller;  // registers other than the arguments
    bool live;
    std::vector<std::string> *calls_log = nullptr;
};

void guest_fault(Guest *g, const char *what, gaddr address) {
    const Machine *m = static_cast<const Machine *>(g->impl);
    fatal(std::string(what) + " at " + hex(address) + (m->live ? " (game)" : " (reference)"));
}

// Runs from c.pc until it returns to kReturnMagic.
bool run_nested(Runtime &rt, AllegrexContext &c, bool live, std::uint64_t budget, std::string *error) {
    ChainOff chain_off(rt);
    const std::uint64_t generation = psprecomp::runtime_thread_switch_generation();
    for (std::uint64_t steps = 0; c.pc != kReturnMagic; ++steps) {
        if (steps >= budget) {
            if (error) *error = "no return after " + std::to_string(budget) + " dispatches, at " + hex(c.pc);
            return false;
        }
        if (!rt.invoke_isolated_aot(c.pc, c)) {
            if (error) *error = "no function at " + hex(c.pc);
            return false;
        }
        if (rt.stopped()) {
            if (error) *error = "runtime stopped: " + rt.stop_reason();
            return false;
        }
        if (live && psprecomp::runtime_thread_switch_generation() != generation)
            fatal("a guest call from decompiled code switched threads at " + hex(c.pc));
    }
    return true;
}

void machine_call(Guest *g, gaddr fn, GuestRegs *r) {
    Machine *m = static_cast<Machine *>(g->impl);
    AllegrexContext c = m->caller;
    load_arguments(c, *r);
    c.gpr[29] = g->sp;
    c.gpr[31] = kReturnMagic;
    c.pc = fn;
    std::string error;
    bool ok;
    if (m->live) {
        portablekit::Kernel &k = portablekit::kernel();
        const bool dispatch = k.dispatch_enabled();
        k.set_dispatch_enabled(false);
        ok = run_nested(m->rt, c, true, ~0ull, &error);
        k.set_dispatch_enabled(dispatch);
    } else {
        ok = run_nested(m->rt, c, false, 3'000'000ull, &error);
    }
    if (!ok) throw std::runtime_error("guest call " + hex(fn) + ": " + error);
    r->v0 = c.gpr[2];
    r->v1 = c.gpr[3];
    r->f0 = c.fpr[0];
}

Guest make_guest(Machine &m, std::uint32_t sp) {
    Guest g{};
    g.ram_size = m.rt.memory().size();
    g.ram = m.rt.memory().raw_pointer(GUEST_RAM_BASE, g.ram_size);
    g.sp = sp;
    g.call = &machine_call;
    g.fault = &guest_fault;
    g.impl = &m;
    return g;
}

// Runs a decompiled function on `c` (its $pc is the function's address on
// entry); on return `c` is as after the original's return, or at the target
// of its tail call.
void run_decomp(Machine &m, AllegrexContext &c, const CallState &in, DecompFunction run, Returns returns) {
    Guest g = make_guest(m, c.gpr[29]);
    GuestRegs out{};
    run(&g, in, out);
    if (returns == Returns::Integer || returns == Returns::Integer64) {
        c.gpr[2] = out.v0;
        c.gpr[3] = out.v1;
    } else if (returns == Returns::Float) {
        c.fpr[0] = out.f0;
    }
    if (g.tail_pending) {
        load_arguments(c, g.tail_regs);
        c.pc = g.tail_fn;
    } else {
        c.pc = c.gpr[31];
    }
}

// ---- The reference runtime --------------------------------------------------

struct HleCall {
    std::string library;
    std::uint32_t nid;
    std::array<std::uint32_t, 4> a;
    std::string text;
    bool operator==(const HleCall &) const = default;
};

struct Reference;
Reference &reference();
void reference_stub(CorpusRuntime &corpus, AllegrexContext &c);

struct Reference {
    std::unique_ptr<Runtime> rt;
    const Library *overlay = nullptr;
    std::vector<HleCall> log;
    std::vector<std::uint8_t> snapshot, after_original;
    std::size_t imports = 0;
    std::uint32_t vcount = 0, microseconds = 0;  // the reference's clocks, from 0 at each run

    void create(const Runtime &live) {
        rt = std::make_unique<Runtime>(live.memory().size());
        psprecomp::register_generated_functions(*rt);
        for (const auto &[address, stub] : harness().stubs)
            rt->register_function(address, &reference_stub, std::string("d012_reference_stub_") + stub.name);
        // Every system call the executable imports answers 0 and is logged.
        try {
            const char *game_dir = portablekit::env("GAME_DIR");
            const std::filesystem::path eboot = std::filesystem::path(game_dir ? game_dir : "game") / "EBOOT.ELF";
            const psprecomp::Elf32Image elf = psprecomp::Elf32Image::from_file(eboot);
            const std::uint32_t load_base = portablekit::game().load_base;
            if (const auto module = elf.find_module_info(live.memory(), load_base)) {
                for (const psprecomp::PspImport &import : elf.scan_imports(live.memory(), *module)) {
                    const std::string library = import.library;
                    const std::uint32_t nid = import.nid;
                    // Clocks advance on every read, so that a wait for a frame or
                    // for time to pass ends; everything else answers 0.
                    const std::string fn = live.nids().resolve(library, nid).value_or("");
                    const int kind = fn.find("Vcount") != std::string::npos || fn.find("WaitVblank") != std::string::npos
                                         ? 1
                                         : (fn.find("GetSystemTime") != std::string::npos ? 2 : 0);
                    rt->register_hle(library, nid, [this, library, nid, kind](Runtime &, AllegrexContext &c) {
                        log.push_back(HleCall{library, nid, {c.gpr[4], c.gpr[5], c.gpr[6], c.gpr[7]}, {}});
                        c.gpr[2] = kind == 1 ? ++vcount : (kind == 2 ? (microseconds += 16683u) : 0u);
                        c.gpr[3] = 0u;
                    });
                    ++imports;
                }
            }
        } catch (const std::exception &e) {
            std::cerr << "[decomp] reference: no imports recorded (" << e.what() << ")\n";
        }
        std::cout << "[decomp] reference runtime: " << rt->function_count() << " functions, " << imports
                  << " imports recorded" << std::endl;
    }

    // Guest RAM and VRAM as in the game, and the game's overlay.
    void load(const Runtime &live) {
        const std::uint32_t size = live.memory().size();
        const std::uint8_t *from = live.memory().raw_pointer(GUEST_RAM_BASE, size);
        snapshot.assign(from, from + size);
        restore();
        const auto &vram = live.memory().vram_bytes();
        if (std::uint8_t *to = rt->memory().raw_pointer(psprecomp::GuestMemory::kVramPhysicalBase, vram.size()))
            std::memcpy(to, vram.data(), vram.size());
        const Library *resident = resident_overlay(live.memory());
        if (resident != overlay) {
            rt->unregister_functions(kWindowBase, kWindowEnd);
            if (resident != nullptr) resident->install(*rt);
            overlay = resident;
        }
        log.clear();
    }
    void restore() {
        vcount = 0;
        microseconds = 0;
        std::memcpy(rt->memory().raw_pointer(GUEST_RAM_BASE, static_cast<std::uint32_t>(snapshot.size())),
                    snapshot.data(), snapshot.size());
    }
    std::uint8_t *ram() { return rt->memory().raw_pointer(GUEST_RAM_BASE, static_cast<std::uint32_t>(snapshot.size())); }
};

Reference &reference() {
    static Reference r;
    return r;
}

void reference_stub(CorpusRuntime &, AllegrexContext &c) {
    const auto found = harness().stubs.find(c.pc);
    if (found == harness().stubs.end()) fatal("no reference stub at " + hex(c.pc));
    const ReferenceStub &stub = found->second;
    HleCall call{stub.name, c.pc, {}, {}};
    for (int i = 0; i < stub.arguments && i < 4; ++i) call.a[static_cast<std::size_t>(i)] = c.gpr[4 + i];
    if (stub.path_first) {
        call.a[0] = 0;
        call.text = reference().rt->memory().read_c_string(c.gpr[4]);
    }
    reference().log.push_back(call);
    c.gpr[2] = found->second.v0;
    c.pc = c.gpr[31];
}

std::string describe(const HleCall &call) {
    std::ostringstream text;
    text << call.library << ":" << hex(call.nid) << "(" << (call.text.empty() ? hex(call.a[0]) : "\"" + call.text + "\"")
         << ", " << hex(call.a[1]) << ", " << hex(call.a[2]) << ", " << hex(call.a[3]) << ")";
    return text.str();
}

// The differential test of one call: the original at `address` against `run`.
void compare(const char *name, std::uint32_t address, Returns returns, DecompFunction run, Runtime &live,
             const AllegrexContext &entry) {
    Harness &h = harness();
    Reference &ref = reference();
    if (!ref.rt) ref.create(live);
    ref.load(live);

    const std::uint32_t sp = entry.gpr[29];
    // The original.
    AllegrexContext a = entry;
    a.gpr[31] = kReturnMagic;
    a.pc = address;
    std::string error;
    const bool original_ok = run_nested(*ref.rt, a, false, 3'000'000ull, &error);
    const std::vector<HleCall> log_a = ref.log;
    if (original_ok) ref.after_original.assign(ref.ram(), ref.ram() + ref.snapshot.size());

    // The decompiled function, from the same state.
    ref.restore();
    ref.log.clear();
    AllegrexContext b = entry;
    b.gpr[31] = kReturnMagic;
    b.pc = address;
    bool decomp_ok = original_ok;
    std::string decomp_error;
    if (original_ok) {
        try {
            Machine m{*ref.rt, entry, false};
            run_decomp(m, b, state_of(entry), run, returns);
            if (b.pc != kReturnMagic) decomp_ok = run_nested(*ref.rt, b, false, 3'000'000ull, &decomp_error);
        } catch (const std::exception &e) {
            decomp_ok = false;
            decomp_error = e.what();
        }
    }
    if (!original_ok || !decomp_ok) {
        ++h.inconclusive;
        std::cout << "[decomp] verify " << name << " (" << hex(address) << "): INCONCLUSIVE: "
                  << (original_ok ? "decompiled: " + decomp_error : "original: " + error) << std::endl;
        return;
    }

    std::vector<std::string> problems;
    if ((returns == Returns::Integer || returns == Returns::Integer64) && (a.gpr[2] != b.gpr[2]))
        problems.push_back("v0 " + hex(a.gpr[2]) + " vs " + hex(b.gpr[2]));
    if (returns == Returns::Integer64 && (a.gpr[3] != b.gpr[3]))
        problems.push_back("v1 " + hex(a.gpr[3]) + " vs " + hex(b.gpr[3]));
    if (returns == Returns::Float && std::memcmp(&a.fpr[0], &b.fpr[0], 4) != 0) problems.push_back("f0 differs");
    for (int r : {16, 17, 18, 19, 20, 21, 22, 23, 28, 29, 30}) {
        if (a.gpr[r] != b.gpr[r])
            problems.push_back("r" + std::to_string(r) + " " + hex(a.gpr[r]) + " vs " + hex(b.gpr[r]));
    }
    for (int f = 20; f < 32; ++f) {
        if (std::memcmp(&a.fpr[f], &b.fpr[f], 4) != 0) problems.push_back("f" + std::to_string(f) + " differs");
    }
    // Memory.
    const std::uint8_t *ram_a = ref.after_original.data();
    const std::uint8_t *ram_b = ref.ram();
    const std::uint8_t *ram_0 = ref.snapshot.data();
    const std::size_t size = ref.snapshot.size();
    const std::size_t dead_lo = std::min<std::size_t>(size, (sp & 0x1FFFFFFFu) - GUEST_RAM_BASE > kDeadStack
                                                                ? (sp & 0x1FFFFFFFu) - GUEST_RAM_BASE - kDeadStack
                                                                : 0u);
    const std::size_t dead_hi = std::min<std::size_t>(size, (sp & 0x1FFFFFFFu) - GUEST_RAM_BASE);
    std::size_t written = 0, differing = 0;
    std::ostringstream first;
    constexpr std::size_t kChunk = 4096;
    for (std::size_t i = 0; i < size; ++i) {
        if (i % kChunk == 0) {
            const std::size_t n = std::min(kChunk, size - i);
            const bool overlaps_dead = i < dead_hi && i + n > dead_lo;
            if (!overlaps_dead && std::memcmp(ram_a + i, ram_b + i, n) == 0 && std::memcmp(ram_a + i, ram_0 + i, n) == 0) {
                i += n - 1;
                continue;
            }
        }
        if (i >= dead_lo && i < dead_hi) continue;
        if (ram_a[i] != ram_0[i]) ++written;
        if (ram_a[i] != ram_b[i]) {
            if (differing < 8)
                first << " " << hex(static_cast<std::uint32_t>(GUEST_RAM_BASE + i)) << ":" << int(ram_a[i]) << "/"
                      << int(ram_b[i]);
            ++differing;
        }
    }
    if (differing != 0) problems.push_back(std::to_string(differing) + " bytes differ:" + first.str());
    if (log_a != ref.log) {
        std::ostringstream text;
        text << "system calls differ (" << log_a.size() << " vs " << ref.log.size() << ")";
        for (std::size_t i = 0; i < std::max(log_a.size(), ref.log.size()) && i < 4; ++i)
            text << " [" << (i < log_a.size() ? describe(log_a[i]) : "-") << " | "
                 << (i < ref.log.size() ? describe(ref.log[i]) : "-") << "]";
        problems.push_back(text.str());
    }

    std::ostringstream line;
    line << "[decomp] verify " << name << " (" << hex(address) << ") a0=" << hex(entry.gpr[4])
         << " a1=" << hex(entry.gpr[5]) << ": ";
    if (problems.empty()) {
        ++h.same;
        line << "SAME (" << written << " bytes written, " << log_a.size() << " system calls";
        if (returns == Returns::Integer) line << ", v0=" << hex(a.gpr[2]);
        if (returns == Returns::Integer64) line << ", v0:v1=" << hex(a.gpr[2]) << ":" << hex(a.gpr[3]);
        line << ")";
    } else {
        ++h.different;
        line << "DIFFERENT:";
        for (const std::string &p : problems) line << " " << p << ";";
    }
    std::cout << line.str() << std::endl;
}

// ---- The overrides in the game -------------------------------------------------

void live_entry(CorpusRuntime &corpus, AllegrexContext &ctx) {
    Runtime &rt = static_cast<Runtime &>(corpus);
    Harness &h = harness();
    const auto found = h.by_address.find(ctx.pc);
    if (found == h.by_address.end()) fatal("no override at " + hex(ctx.pc));
    Registered &reg = h.overrides[found->second];
    ++reg.calls;
    if (h.verify && reg.verified < h.verify_limit) {
        ++reg.verified;
        if (reg.o.probes != nullptr) reg.o.probes(state_of(ctx));
        compare(reg.o.name, reg.o.address, reg.o.returns, reg.o.check != nullptr ? reg.o.check : reg.o.run, rt, ctx);
    }
    const CallState in = state_of(ctx);
    Machine m{rt, ctx, true};
    try {
        run_decomp(m, ctx, in, reg.o.run, reg.o.returns);
    } catch (const std::exception &e) {
        fatal(std::string(reg.o.name) + ": " + e.what());
    }
}

void register_set(Runtime &rt, Module module) {
    Harness &h = harness();
    for (const Registered &reg : h.overrides) {
        if (reg.o.module != module || !reg.o.live || h.verify_only) continue;
        rt.register_function(reg.o.address, &live_entry, std::string("d012_decomp_") + reg.o.name);
    }
}

void rearm(Runtime &rt) {
    Harness &h = harness();
    const Library *resident = resident_overlay(rt.memory());
    const bool battle = resident != nullptr && resident->name == "battle";
    if (battle && (h.rearm_pending || resident != h.armed_overlay)) {
        register_set(rt, Module::BattleOverlay);
        std::cout << "[decomp] overrides in the battle overlay registered" << std::endl;
    }
    h.armed_overlay = resident;
    h.rearm_pending = false;
}

void post_dispatch(Runtime &rt, AllegrexContext &, std::uint32_t, std::int32_t) {
    Harness &h = harness();
    // After an overlay was installed, and now and then in case one was
    // installed without a dispatch miss.
    if (h.rearm_pending || (++h.dispatches & 0xFFFFu) == 0u) rearm(rt);
}

bool code_miss(Runtime &rt, AllegrexContext &, std::uint32_t pc) {
    Harness &h = harness();
    h.live = &rt;
    if (!h.installed_main) {
        h.installed_main = true;
        register_set(rt, Module::Main);
        psprecomp::set_runtime_post_dispatch_hook(&post_dispatch);
        std::size_t live = 0, replaced = 0;
        for (const Registered &reg : h.overrides) {
            live += reg.o.live && !h.verify_only;
            replaced += reg.o.live && !h.verify_only && reg.o.check != nullptr;
        }
        std::cout << "[decomp] " << h.overrides.size() << " decompiled functions, " << live << " in place of the "
                  << "originals (" << replaced << " of them port replacements)"
                  << (h.verify ? "; differential test on" : "") << std::endl;
    }
    if (pc >= kWindowBase && pc < kWindowEnd) h.rearm_pending = true;
    return false;  // PortableKit installs the overlay as before
}

bool flag(const char *name) {
    const char *value = std::getenv(name);
    return value != nullptr && *value != '\0' && std::strcmp(value, "0") != 0;
}

} // namespace

void add_overrides(const Override *overrides, std::size_t count, bool enable) {
    Harness &h = harness();
    h.verify = h.verify || (enable && flag("D012_DECOMP_VERIFY"));
    h.verify_only = flag("D012_DECOMP_VERIFY_ONLY");
    if (const char *limit = std::getenv("D012_DECOMP_VERIFY_LIMIT")) h.verify_limit = std::strtoull(limit, nullptr, 0);
    if (!enable) return;
    h.enabled = true;
    for (std::size_t i = 0; i < count; ++i) {
        h.by_address[overrides[i].address] = h.overrides.size();
        h.overrides.push_back(Registered{overrides[i]});
    }
    portablekit::set_code_miss_hook(&code_miss);
}

bool verifying() { return harness().verify; }

void add_reference_stubs(const ReferenceStub *stubs, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i) harness().stubs[stubs[i].address] = stubs[i];
}

std::uint32_t live_load32(std::uint32_t address) {
    Harness &h = harness();
    return h.live != nullptr ? h.live->memory().load32(address) : 0u;
}

void verify_probe(const char *name, std::uint32_t address, Returns returns, DecompFunction run,
                  const CallState &caller, const GuestRegs &arguments) {
    Harness &h = harness();
    if (!h.verify) return;
    if (h.probes_done[name]++ >= h.verify_limit) return;
    // The probe runs from the state of the current call, with its own arguments.
    AllegrexContext entry{};
    entry.gpr = caller.gpr;
    entry.fpr = caller.fpr;
    // The VFPU prefixes as between any two instructions (each one consumes
    // them): the identity swizzle, no destination saturation or mask. Zero
    // would read every source lane as x.
    entry.vfpu_ctrl[0] = 0xE4u;
    entry.vfpu_ctrl[1] = 0xE4u;
    entry.vfpu_ctrl[2] = 0u;
    load_arguments(entry, arguments);
    if (h.live == nullptr) return;
    compare(name, address, returns, run, *h.live, entry);
}

} // namespace d012::decomp
