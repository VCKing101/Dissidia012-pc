// LIBSUPPREACC.PRX, the game's own support library on the disc.
//
// PortableKit answers sceKernelLoadModule for Sony's stock modules without
// loading anything, because their exports are HLE. This one is not a stock
// module: it is a plain (unencrypted) user PRX in PSP_GAME/USRDIR/data/module
// that exports the "scesupPreAcc" library, and nothing in PortableKit serves
// those functions. So, for this one file, sceKernelLoadModule loads it for
// real: into a block of the user partition, relocated, with its own imports
// bound to PortableKit's HLE. Main's eight scesupPreAcc imports then jump to
// its exports and run its code like the game's own (interpreted until a
// corpus covers it).
//
// D012_NO_SUPPREACC=1 restores PortableKit's behaviour: the load is only
// acknowledged and the imports are logging stubs that answer 0.
//
// MIT licence; see port/README.md.

#include "d012_hle.hpp"

#include "hle/hle_common.hpp"
#include "profile.hpp"

#include "psprecomp/common.hpp"
#include "psprecomp/elf32.hpp"
#include "psprecomp/runtime.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace d012 {
namespace {

using psprecomp::AllegrexContext;
using portablekit::arg;
using portablekit::kernel;
using psprecomp::hex32;
using psprecomp::Runtime;

constexpr std::uint32_t kLoadModuleNid = 0x977DE386u;  // ModuleMgrForUser::sceKernelLoadModule
constexpr std::uint32_t kStartModuleNid = 0x50F0C1ECu; // ModuleMgrForUser::sceKernelStartModule
constexpr const char *kLibrary = "scesupPreAcc";
// The library's functions main imports (five called from main, three from the
// extra overlay through main's stubs). Their names are not known.
constexpr std::uint32_t kImportedNids[] = {0x110E318Bu, 0x13AE25B3u, 0x28C5F696u, 0x2EC3F4D9u,
                                           0x348BA3E2u, 0x86DEBD66u, 0xA0EAF444u, 0xB03FF882u};

struct LoadedPrx {
    std::int32_t uid{};
    std::uint32_t base{};
    std::uint32_t entry{}; // 0: no module_start
    std::map<std::uint32_t, std::uint32_t> exports; // scesupPreAcc NID -> address
};

std::unique_ptr<LoadedPrx> &loaded() {
    static std::unique_ptr<LoadedPrx> prx;
    return prx;
}

bool disabled() { return portablekit::env_set("NO_SUPPREACC"); }

bool is_suppreacc_path(std::string path) {
    std::transform(path.begin(), path.end(), path.begin(), [](unsigned char c) { return std::tolower(c); });
    const std::string name = "libsuppreacc.prx";
    return path.size() >= name.size() && path.compare(path.size() - name.size(), name.size(), name) == 0;
}

// Reads the export entries of a loaded module: name, version and attributes,
// length in words, variable and function counts, then the table of NIDs and
// the values in the same order.
std::map<std::uint32_t, std::uint32_t> read_exports(const psprecomp::GuestMemory &memory,
                                                    const psprecomp::PspModuleInfo &module,
                                                    const std::string &library) {
    std::map<std::uint32_t, std::uint32_t> exports;
    for (std::uint32_t cursor = module.ent_top; cursor + 16u <= module.ent_end;) {
        const std::uint32_t name = memory.load32(cursor);
        const std::uint32_t length_words = memory.load8(cursor + 8u);
        const std::uint32_t variables = memory.load8(cursor + 9u);
        const std::uint32_t functions = memory.load16(cursor + 10u);
        const std::uint32_t table = memory.load32(cursor + 12u);
        if (name != 0u && portablekit::read_cstring(memory, name, 64u) == library) {
            const std::uint32_t total = functions + variables;
            for (std::uint32_t i = 0; i < functions; ++i)
                exports[memory.load32(table + i * 4u)] = memory.load32(table + (total + i) * 4u);
        }
        cursor += std::max<std::uint32_t>(length_words, 4u) * 4u;
    }
    return exports;
}

// Loads the PRX image into guest memory. Returns null, having said why, when
// it cannot.
std::unique_ptr<LoadedPrx> load(Runtime &runtime, const std::string &path) {
    std::vector<std::uint8_t> bytes(256u * 1024u);
    const std::int64_t count = portablekit::read_guest_file(path, 0u, bytes.data(), bytes.size());
    if (count <= 0) {
        std::cerr << "[d012] cannot read " << path << "\n";
        return nullptr;
    }
    bytes.resize(static_cast<std::size_t>(count));
    const psprecomp::Elf32Image elf = psprecomp::Elf32Image::from_bytes(std::move(bytes), path);
    if (!elf.is_psp_prx()) {
        std::cerr << "[d012] " << path << " is not a plain PRX\n";
        return nullptr;
    }
    std::uint32_t image_size = 0u;
    for (const psprecomp::ElfSegment &segment : elf.segments())
        if (segment.type == 1u) image_size = std::max(image_size, segment.vaddr + segment.memory_size);

    auto prx = std::make_unique<LoadedPrx>();
    // The low end of the user partition, where the PSP's loader places a
    // module.
    const std::int32_t block = kernel().allocate_block("libsuppreacc.prx", 0u, image_size, 0u);
    const portablekit::MemoryBlock *memory_block = block > 0 ? kernel().find_block(block) : nullptr;
    if (memory_block == nullptr) {
        std::cerr << "[d012] no room for " << path << " (" << image_size << " bytes)\n";
        return nullptr;
    }
    prx->base = memory_block->address;
    const psprecomp::RelocationStats relocations = elf.load_and_relocate(runtime.memory(), prx->base);
    const auto module = elf.find_module_info(runtime.memory(), prx->base);
    if (!module) {
        std::cerr << "[d012] no module info in " << path << "\n";
        return nullptr;
    }
    std::size_t bound = 0u;
    const std::vector<psprecomp::PspImport> imports = elf.scan_imports(runtime.memory(), *module);
    for (const psprecomp::PspImport &import : imports)
        if (runtime.register_import_stub(import.stub_address, import.library, import.nid)) ++bound;
    prx->exports = read_exports(runtime.memory(), *module, kLibrary);
    prx->entry = elf.entry() == 0xFFFFFFFFu ? 0u : elf.runtime_entry(prx->base);
    prx->uid = kernel().allocate_uid();
    std::cout << "[d012] loaded " << module->name << " at " << hex32(prx->base) << " (" << image_size
              << " bytes, " << relocations.total << " relocations, " << bound << " of " << imports.size()
              << " imports bound, " << prx->exports.size() << " " << kLibrary << " exports, gp "
              << hex32(module->gp) << ")\n";
    return prx;
}

} // namespace

void register_suppreacc(portablekit::HleRegistrar &hle) {
    if (disabled()) return;

    const portablekit::HleFunction stock_load = hle.bound_function("ModuleMgrForUser", kLoadModuleNid);
    hle.bind("ModuleMgrForUser", kLoadModuleNid, [stock_load](Runtime &rt, AllegrexContext &ctx) {
        const std::string path = portablekit::read_cstring(rt.memory(), arg(ctx, 0), 256u);
        if (!is_suppreacc_path(path)) return stock_load(rt, ctx);
        if (!loaded()) loaded() = load(rt, path);
        if (!loaded()) return stock_load(rt, ctx);
        kernel().finish(ctx, static_cast<std::uint32_t>(loaded()->uid));
    });

    const portablekit::HleFunction stock_start = hle.bound_function("ModuleMgrForUser", kStartModuleNid);
    hle.bind("ModuleMgrForUser", kStartModuleNid, [stock_start](Runtime &rt, AllegrexContext &ctx) {
        const LoadedPrx *prx = loaded().get();
        if (prx == nullptr || static_cast<std::int32_t>(arg(ctx, 0)) != prx->uid) return stock_start(rt, ctx);
        // This PRX has no module_start (its e_entry is -1); a module that had
        // one would need it called here.
        if (prx->entry != 0u)
            std::cerr << "[d012] libsuppreacc.prx has a module_start at " << hex32(prx->entry)
                      << " that is not run\n";
        if (arg(ctx, 3) != 0u) rt.memory().store32(arg(ctx, 3), 0u);
        kernel().finish(ctx, static_cast<std::uint32_t>(prx->uid));
    });

    for (const std::uint32_t nid : kImportedNids) {
        hle.bind(kLibrary, nid, [nid](Runtime &, AllegrexContext &ctx) {
            const LoadedPrx *prx = loaded().get();
            const std::uint32_t target = prx != nullptr && prx->exports.contains(nid) ? prx->exports.at(nid) : 0u;
            if (target == 0u) {
                portablekit::log_once("suppreacc:" + hex32(nid),
                                      std::string("[d012] ") + kLibrary + "::" + hex32(nid) +
                                          " called with no library loaded; answering 0");
                kernel().finish(ctx, 0u);
                return;
            }
            // A tail call: the export runs with the caller's arguments and
            // returns to the caller's ra.
            ctx.pc = target;
        });
    }
}

} // namespace d012
