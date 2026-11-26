// DLC packs: the folders of <data dir>/dlc, served to the game as files of
// ms0:/PSP/GAME/<disc id>. Which file goes where is d012_dlc_packs_plan.cpp;
// this reads the folders, says what it did in the log and serves the files. A
// file that is really on the memory stick is never replaced. The guide is
// docs/port/dlc-packs.md. MIT licence; see port/README.md.

#include "d012_dlc_packs.hpp"

#include "d012_dlc.hpp"
#include "d012_dlc_packs_plan.hpp"

#include "install/user_data.hpp"
#include "portablekit/hle_extension.hpp"
#include "profile.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace d012::dlc_packs {
namespace {

namespace fs = std::filesystem;
using portablekit::hle_extension::FileFilter;

std::string utf8(const fs::path &path) { return portablekit::install::path_to_utf8(path); }

std::string lower(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return std::tolower(c); });
    return text;
}

fs::path packs_folder() {
    if (const char *dir = portablekit::env("DLC_PACKS_DIR"); dir != nullptr && *dir != '\0')
        return portablekit::install::path_from_utf8(dir);
    try {
        return portablekit::install::user_data_directory() / "dlc";
    } catch (const std::exception &) {
        return {};
    }
}

std::vector<std::uint8_t> read_whole(const SourceFile &file) {
    std::ifstream in(file.path, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

// Every folder with .edat files under `root` is a pack, in the order of their
// names, so the same folders always give the same slots.
std::vector<Pack> find_packs(const fs::path &root) {
    std::map<std::string, Pack> found;
    std::set<std::string> raw;
    std::error_code ec;
    for (fs::recursive_directory_iterator it(root, fs::directory_options::skip_permission_denied, ec), end;
         !ec && it != end; it.increment(ec)) {
        if (!it->is_regular_file(ec)) continue;
        const fs::path relative = it->path().parent_path().lexically_relative(root);
        const std::string name = relative == "." ? utf8(root.filename()) : utf8(relative);
        const std::string extension = lower(utf8(it->path().extension()));
        if (extension == ".gmo" || extension == ".gim") raw.insert(name);
        if (extension != ".edat") continue;
        const auto crc = crc_of_file_name(utf8(it->path().filename()));
        if (!crc) {
            std::cerr << "[dlc] warning: \"" << name << "\": " << utf8(it->path().filename())
                      << " is left out: a DLC file's name is 8 hexadecimal digits and .edat\n";
            continue;
        }
        Pack &pack = found[lower(name)];
        pack.name = name;
        pack.files.push_back({*crc, static_cast<std::uint64_t>(it->file_size(ec)), it->path()});
    }
    for (const std::string &name : raw)
        if (!found.contains(lower(name)))
            std::cerr << "[dlc] warning: \"" << name
                      << "\" has .gmo or .gim files and no .edat files: make them into DLC with DissDlcToolkit first\n";
    std::vector<Pack> packs;
    for (auto &[key, pack] : found) packs.push_back(std::move(pack));
    return packs;
}

Pack installed_files() {
    Pack installed;
    std::error_code ec;
    for (const auto &entry : fs::directory_iterator(dlc::folder(), ec)) {
        if (!entry.is_regular_file(ec)) continue;
        if (const auto crc = crc_of_file_name(utf8(entry.path().filename())))
            installed.files.push_back({*crc, static_cast<std::uint64_t>(entry.file_size(ec)), entry.path()});
    }
    return installed;
}

class TableFile final : public FileFilter {
public:
    explicit TableFile(const std::vector<std::uint8_t> &bytes) : bytes_(bytes) {}
    [[nodiscard]] std::uint64_t size() const override { return bytes_.size(); }
    std::size_t read(std::uint64_t offset, std::uint8_t *destination, std::size_t length) override {
        if (offset >= bytes_.size()) return 0u;
        const std::size_t count = std::min<std::uint64_t>(length, bytes_.size() - offset);
        std::copy_n(bytes_.begin() + static_cast<std::ptrdiff_t>(offset), count, destination);
        return count;
    }

private:
    const std::vector<std::uint8_t> &bytes_;
};

// Opened at the first read: the game asks whether a file exists far more often
// than it reads one.
class PackFile final : public FileFilter {
public:
    explicit PackFile(const Served &file) : file_(file) {}
    [[nodiscard]] std::uint64_t size() const override { return file_.size; }
    std::size_t read(std::uint64_t offset, std::uint8_t *destination, std::size_t length) override {
        if (!in_.is_open()) in_.open(file_.path, std::ios::binary);
        in_.clear();
        in_.seekg(static_cast<std::streamoff>(offset));
        in_.read(reinterpret_cast<char *>(destination), static_cast<std::streamsize>(length));
        return static_cast<std::size_t>(in_.gcount());
    }

private:
    const Served &file_;
    std::ifstream in_;
};

std::unordered_map<std::uint32_t, Served> &served() {
    static std::unordered_map<std::uint32_t, Served> files;
    return files;
}

std::shared_ptr<FileFilter> serve_file(std::string_view path, std::uint32_t fd) {
    if (fd != 0u) return nullptr;  // a file that is there stays
    const std::size_t slash = path.rfind('/');
    if (slash == std::string_view::npos) return nullptr;
    const auto crc = crc_of_file_name(path.substr(slash + 1u));
    if (!crc) return nullptr;
    static const std::string folder = lower("ms0:/PSP/GAME/" + std::string(portablekit::active_disc_id()) + "/");
    if (lower(std::string(path.substr(0u, slash + 1u))) != folder) return nullptr;
    const auto found = served().find(*crc);
    if (found == served().end()) return nullptr;
    if (found->second.rewritten) return std::make_shared<TableFile>(found->second.bytes);
    return std::make_shared<PackFile>(found->second);
}

} // namespace

void install(portablekit::HleRegistrar &) {
    static bool installed = false;
    if (installed) return;
    installed = true;
    if (portablekit::env("NO_DLC_PACKS") != nullptr) return;
    const fs::path root = packs_folder();
    std::error_code ec;
    if (root.empty() || !fs::is_directory(root, ec)) return;
    const std::vector<Pack> packs = find_packs(root);
    if (packs.empty()) return;
    Plan result = plan(packs, installed_files(), read_whole);
    for (const std::string &line : result.moved) std::cout << "[dlc] pack " << line << "\n";
    for (const std::string &line : result.problems) std::cerr << "[dlc] warning: pack " << line << "\n";
    std::map<std::string, int> counts;
    for (Served &file : result.files) {
        ++counts[file.pack];
        const std::uint32_t crc = file.crc;
        served().emplace(crc, std::move(file));
    }
    for (const Pack &pack : packs) std::cout << "[dlc] pack \"" << pack.name << "\": " << counts[pack.name] << " files\n";
    portablekit::hle_extension::add_file_provider(serve_file);
}

std::optional<std::vector<std::uint8_t>> served_file(std::uint32_t crc) {
    const auto found = served().find(crc);
    if (found == served().end()) return std::nullopt;
    if (found->second.rewritten) return found->second.bytes;
    std::ifstream in(found->second.path, std::ios::binary);
    if (!in) return std::nullopt;
    return std::vector<std::uint8_t>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

} // namespace d012::dlc_packs
