// Adding content on Android (d012_android_import.hpp).
//
// Import mods: the picked folder is one mod (it holds mod.json) or a folder of
// mods (its folders that hold mod.json). Each is copied into the mods folder
// (d012_mods.cpp, docs/port/mod-folder.md) as a folder of its own: a single
// mod under its id, mods in a folder under their folders' names. A mod already
// there under that name is replaced only once the new copy is complete.
//
// Import DLC: every .edat file in the picked folder and four levels of folders
// below it, so a memory stick's root, its PSP or GAME folder, or the
// ULUS10566 folder itself all work; when some are in a folder named for the
// disc being played (portablekit::active_disc_id()), only those. They are
// copied into ms0:/PSP/GAME/<disc id> (d012_dlc.cpp), the one folder the game
// reads DLC from.
//
// The game reads both when it starts, so the rows offer to restart it.
// MIT licence; see port/README.md.

#include "d012_android_import.hpp"

#if defined(PORTABLEKIT_ANDROID_APP)
#include "d012_dlc.hpp"
#include "d012_json.hpp"
#include "d012_mods.hpp"

#include "install/user_data.hpp"
#include "platform/android_jni.hpp"
#include "profile.hpp"
#include "ui/ui.hpp"
#include "ui/widgets.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <array>
#include <cctype>
#include <cerrno>
#include <fstream>
#include <iostream>
#include <iterator>
#include <mutex>
#include <thread>
#include <vector>
#endif

namespace d012::android_import {

#if defined(PORTABLEKIT_ANDROID_APP)
namespace {

namespace fs = std::filesystem;
namespace android = portablekit::android;
namespace ui = portablekit::ui;

// Room left on the device after a copy.
constexpr std::uint64_t kSpareBytes = 64ull << 20;

std::string utf8(const fs::path &path) { return portablekit::install::path_to_utf8(path); }

std::string lower(std::string text) {
    for (char &c : text) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return text;
}

std::string megabytes(std::uint64_t bytes) { return std::to_string((bytes + (1u << 19)) >> 20) + " MB"; }

// A folder name made of a mod id or a picked folder's name: letters, digits,
// '.', '-' and '_' kept, the rest '_'.
std::string folder_name(const std::string &name) {
    std::string out;
    for (const char c : name)
        out.push_back(std::isalnum(static_cast<unsigned char>(c)) || c == '.' || c == '-' || c == '_' ? c : '_');
    while (!out.empty() && out.front() == '.') out.erase(out.begin());
    return out.empty() ? "imported-mod" : out;
}

struct File {
    std::string uri;
    fs::path relative;  // where it goes, under the target folder
};

// The files under the folder document `folder`, `depth` levels of folders
// down, with their paths relative to it.
bool walk(const std::string &folder, const fs::path &prefix, int depth, std::vector<File> &files) {
    const auto entries = android::list_folder(folder);
    if (!entries) return false;
    for (const android::Entry &entry : *entries) {
        const fs::path relative = prefix / portablekit::install::path_from_utf8(entry.name);
        if (!entry.directory) files.push_back({entry.uri, relative});
        else if (depth > 0 && !walk(entry.uri, relative, depth - 1, files)) return false;
    }
    return true;
}

// --- A job: one import at a time, copying on a thread of its own ---------

enum class Kind { Mods, Dlc };

struct Job {
    std::mutex lock;
    std::thread worker;
    std::atomic<bool> running{false};
    Progress progress;
    std::atomic<int> files_done{0};
    std::atomic<int> files_total{0};
    Kind kind = Kind::Mods;
    std::string message;   // the last import's outcome, for the player
    bool failed = false;
    bool restart = false;  // something was copied that the game reads at start
};

Job &job() {
    static Job instance;
    return instance;
}

void finish(const std::string &message, bool failed, bool restart) {
    Job &j = job();
    {
        std::lock_guard guard(j.lock);
        j.message = message;
        j.failed = failed;
        j.restart = j.restart || restart;
    }
    std::cout << "[import] " << message << std::endl;
    j.running = false;
}

// Copies `files` under `target` (made first), counting them in the job.
std::optional<std::string> copy_files(const std::vector<File> &files, const fs::path &target) {
    Job &j = job();
    std::error_code ec;
    fs::create_directories(target, ec);
    if (ec) return "cannot make " + utf8(target);
    for (const File &file : files) {
        if (j.progress.cancel) return "Cancelled";
        const fs::path to = target / file.relative;
        fs::create_directories(to.parent_path(), ec);
        if (const auto error = copy_document(file.uri, to, j.progress, false)) return error;
        ++j.files_done;
    }
    return std::nullopt;
}

// One mod's folder document and the name it gets in the mods folder.
struct Mod {
    std::string uri;
    std::string name;
    std::vector<File> files;
};

// The id in a mod's mod.json, read through a copy in `scratch`.
std::string mod_id(const std::string &manifest_uri, const fs::path &scratch) {
    Progress ignored;
    const fs::path copy = scratch / "mod.json";
    if (copy_document(manifest_uri, copy, ignored)) return {};
    std::ifstream in(copy, std::ios::binary);
    const std::string text{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    std::string error;
    const auto root = json::parse(text, error);
    const json::Value *id = root ? root->find("id") : nullptr;
    return id != nullptr && id->is(json::Value::Kind::String) ? id->text : std::string();
}

void import_mods(const std::string &tree) {
    const fs::path mods = mods::folder();
    if (mods.empty()) return finish("Mods: the game has no mods folder", true, false);
    const fs::path scratch = mods / ".import";
    std::error_code ec;
    fs::remove_all(scratch, ec);
    fs::create_directories(scratch, ec);
    const auto has_manifest = [](const std::vector<android::Entry> &entries) -> const android::Entry * {
        for (const android::Entry &e : entries)
            if (!e.directory && lower(e.name) == "mod.json") return &e;
        return nullptr;
    };
    const std::string root = android::tree_root(tree);
    const auto entries = android::list_folder(root);
    if (!entries) return finish("Mods: the folder cannot be read. Choose it again.", true, false);
    std::vector<Mod> found;
    if (const android::Entry *manifest = has_manifest(*entries)) {
        std::string id = mod_id(manifest->uri, scratch);
        found.push_back({root, folder_name(id.empty() ? "imported-mod" : id), {}});
    } else {
        for (const android::Entry &e : *entries) {
            if (!e.directory) continue;
            const auto inside = android::list_folder(e.uri);
            if (inside && has_manifest(*inside) != nullptr) found.push_back({e.uri, folder_name(e.name), {}});
        }
    }
    if (found.empty()) {
        fs::remove_all(scratch, ec);
        return finish("Mods: no mod found. Choose a mod's folder (the one with mod.json in it) or a folder of mods. "
                      "Unzip a downloaded mod in the Files app first.",
                      true, false);
    }
    Job &j = job();
    int total = 0;
    for (Mod &mod : found) {
        if (!walk(mod.uri, {}, 8, mod.files))
            return finish("Mods: the folder cannot be read. Choose it again.", true, false);
        total += static_cast<int>(mod.files.size());
    }
    j.files_total = total;
    std::string names;
    for (const Mod &mod : found) {
        const fs::path staged = scratch / mod.name;
        if (const auto error = copy_files(mod.files, staged)) {
            fs::remove_all(scratch, ec);
            return finish("Mods: " + *error, true, !names.empty());
        }
        const fs::path target = mods / mod.name;
        fs::remove_all(target, ec);
        fs::rename(staged, target, ec);
        if (ec) {
            fs::remove_all(scratch, ec);
            return finish("Mods: cannot move " + mod.name + " into the mods folder", true, !names.empty());
        }
        names += (names.empty() ? "" : ", ") + mod.name;
    }
    fs::remove_all(scratch, ec);
    finish("Mods: imported " + names + ". Restart the game to load " + (found.size() == 1u ? "it." : "them."), false,
           true);
}

bool encrypted(const fs::path &file) {
    std::array<char, 8> head{};
    std::ifstream in(file, std::ios::binary);
    return in.read(head.data(), head.size()) && std::string(head.data(), head.size()) == std::string("\0PSPEDAT", 8);
}

void import_dlc(const std::string &tree) {
    const fs::path target = dlc::folder();
    if (target.empty()) return finish("DLC: the game has no memory stick folder", true, false);
    std::vector<File> all;
    if (!walk(android::tree_root(tree), {}, 4, all))
        return finish("DLC: the folder cannot be read. Choose it again.", true, false);
    const std::string disc_id = lower(portablekit::active_disc_id());
    std::vector<File> edat;
    bool for_this_disc = false;
    for (const File &file : all) {
        if (lower(utf8(file.relative.extension())) != ".edat") continue;
        const bool ours = lower(utf8(file.relative.parent_path().filename())) == disc_id;
        if (ours && !for_this_disc) edat.clear();
        for_this_disc = for_this_disc || ours;
        if (ours || !for_this_disc) edat.push_back({file.uri, file.relative.filename()});
    }
    if (edat.empty())
        return finish("DLC: no .edat files in that folder. Choose the folder that holds them, such as PSP/GAME/" +
                          std::string(portablekit::active_disc_id()) + ".",
                      true, false);
    job().files_total = static_cast<int>(edat.size());
    if (const auto error = copy_files(edat, target)) return finish("DLC: " + *error, true, job().files_done > 0);
    int locked = 0;
    for (const File &file : edat) locked += encrypted(target / file.relative) ? 1 : 0;
    std::string message = "DLC: copied " + std::to_string(edat.size()) + (edat.size() == 1u ? " file." : " files.");
    if (locked != 0)
        message += " " + std::to_string(locked) + (locked == 1 ? " is" : " are") +
                   " still encrypted as a PSP keeps them; the game may not read " + (locked == 1 ? "it." : "them.");
    finish(message + " Restart the game to use it.", false, true);
}

void start(Kind kind) {
    Job &j = job();
    if (j.running) return;
    // The picker blocks this thread until the player has chosen.
    const std::optional<std::string> tree = android::pick_folder();
    if (!tree) return;
    std::cout << "[import] " << (kind == Kind::Mods ? "mods" : "DLC") << " from " << *tree << std::endl;
    if (j.worker.joinable()) j.worker.join();
    j.kind = kind;
    j.progress.done = 0;
    j.progress.total = 0;
    j.progress.cancel = false;
    j.files_done = 0;
    j.files_total = 0;
    {
        std::lock_guard guard(j.lock);
        j.message.clear();
        j.failed = false;
    }
    j.running = true;
    j.worker = std::thread([kind, uri = *tree] {
        try {
            if (kind == Kind::Mods) import_mods(uri);
            else import_dlc(uri);
        } catch (const std::exception &error) {
            finish(std::string(kind == Kind::Mods ? "Mods: " : "DLC: ") + error.what(), true, false);
        }
    });
}

void system_rows() {
    Job &j = job();
    ui::section("Add content");
    ui::RowOptions mods_row;
    mods_row.description = "Choose a mod's folder (the one with mod.json in it) or a folder of mods in Android's file "
                           "picker. They are copied into the game. Unzip a downloaded mod in the Files app first.";
    mods_row.disabled = j.running;
    if (ui::button_row("Import mods…", mods_row)) start(Kind::Mods);
    ui::RowOptions dlc_row;
    dlc_row.description = "Choose the folder that holds the DLC's .edat files, such as PSP/GAME/" +
                          std::string(portablekit::active_disc_id()) + " from a PSP or PPSSPP. They are copied into the game.";
    dlc_row.disabled = j.running;
    if (ui::button_row("Import DLC…", dlc_row)) start(Kind::Dlc);
    if (j.running) {
        const int total = j.files_total;
        const int done = j.files_done;
        const float fraction = total > 0 ? static_cast<float>(done) / static_cast<float>(total) : 0.0f;
        ui::progress_bar(fraction, total > 0 ? std::to_string(done) + " of " + std::to_string(total) + " files, " +
                                                   megabytes(j.progress.done)
                                             : std::string("Looking through the folder…"));
        ui::RowOptions cancel;
        cancel.description = "Stops the copy. Files already copied stay.";
        if (ui::button_row("Cancel", cancel)) j.progress.cancel = true;
        return;
    }
    std::string message;
    bool failed = false;
    bool restart = false;
    {
        std::lock_guard guard(j.lock);
        message = j.message;
        failed = j.failed;
        restart = j.restart;
    }
    if (!message.empty()) ui::paragraph(message, failed ? ui::colors::kDanger : ui::colors::kText);
    if (restart) {
        ui::RowOptions again;
        again.description = "Closes the game and starts it again, to read what was added. Progress since your last "
                            "save is lost.";
        if (ui::button_row("Restart the game", again)) android::relaunch();
    }
}

} // namespace

std::optional<std::string> copy_document(const std::string &uri, const fs::path &target, Progress &progress,
                                         bool set_total) {
    const int fd = android::open_document(uri, "r");
    if (fd < 0) return "Android would not let the game read " + utf8(target.filename()) + ". Choose it again.";
    struct stat info {};
    const std::uint64_t size = ::fstat(fd, &info) == 0 ? static_cast<std::uint64_t>(info.st_size) : 0u;
    if (set_total) progress.total = size;
    std::error_code ec;
    fs::create_directories(target.parent_path(), ec);
    if (const fs::space_info space = fs::space(target.parent_path(), ec); !ec && size + kSpareBytes > space.available) {
        ::close(fd);
        return "Not enough free space: " + utf8(target.filename()) + " needs " + megabytes(size) + " and " +
               megabytes(space.available) + " is free.";
    }
    const fs::path partial = fs::path(target).concat(".part");
    std::optional<std::string> error;
    {
        std::ofstream out(partial, std::ios::binary | std::ios::trunc);
        std::vector<char> buffer(1u << 20);
        while (!error) {
            if (progress.cancel) {
                error = "Cancelled";
                break;
            }
            const ssize_t got = ::read(fd, buffer.data(), buffer.size());
            if (got < 0 && errno == EINTR) continue;
            if (got < 0) error = "Reading " + utf8(target.filename()) + " failed. Choose it again.";
            if (got <= 0) break;
            out.write(buffer.data(), got);
            if (!out) error = "Writing " + utf8(target.filename()) + " failed. Check the free space.";
            progress.done += static_cast<std::uint64_t>(got);
        }
        out.close();
        if (!error && !out) error = "Writing " + utf8(target.filename()) + " failed. Check the free space.";
    }
    ::close(fd);
    if (!error) {
        fs::rename(partial, target, ec);
        if (ec) error = "Cannot finish " + utf8(target.filename()) + ".";
    }
    if (error) fs::remove(partial, ec);
    return error;
}

void install(portablekit::HleRegistrar &) {
    static bool installed = false;
    if (installed) return;
    installed = true;
    ui::add_system_rows(&system_rows);
}

#else

std::optional<std::string> copy_document(const std::string &, const std::filesystem::path &, Progress &, bool) {
    return "Android only";
}

void install(portablekit::HleRegistrar &) {}

#endif

} // namespace d012::android_import
