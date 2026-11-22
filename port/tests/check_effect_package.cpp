// Checks the C++ effect package (host/d012_effect_package.cpp) against what
// the Python (tools/effect_package.py) wrote: check_effect_package <dev dir>.
// <dev dir>/ref/fep is what tools/original_moves_refs.py puts there (game
// data; it is never in the repository):
//
//   base.fep        the package the copies go into
//   <source>.fep    the packages effects are copied from
//   steps.json      the copies in order: [{"source", "source_name", "name", "index"}]
//   after.fep       base.fep after them
//
// Every .fep there must read and write back byte for byte. Then the steps are
// done on base.fep: each copy must return its "index" and leave the name
// there, and the package written after the last must be after.fep. One line
// per check; the exit code is 0 only when all of them hold. Built on its own:
//
//   g++ -std=c++20 -O2 -Iport/host port/tests/check_effect_package.cpp
//       port/host/d012_effect_package.cpp port/host/d012_json.cpp -o check_effect_package
//
// MIT licence; see port/README.md.

#include "d012_effect_package.hpp"
#include "d012_json.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace {

namespace fep = d012::effect_package;
namespace json = d012::json;
using Bytes = std::vector<std::uint8_t>;

int g_checks = 0;
int g_failed = 0;

void report(bool passed, const std::string &what) {
    ++g_checks;
    if (!passed) ++g_failed;
    std::printf("%s %s\n", passed ? "ok  " : "FAIL", what.c_str());
}

std::optional<Bytes> read_file(const std::filesystem::path &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return std::nullopt;
    const std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return Bytes(text.begin(), text.end());
}

// Where two files first differ, for the line of a check that failed.
std::string difference(const Bytes &got, const Bytes &want) {
    const std::size_t shared = std::min(got.size(), want.size());
    std::size_t at = 0u;
    while (at < shared && got[at] == want[at]) ++at;
    if (at == shared && got.size() == want.size()) return {};
    return ": " + std::to_string(got.size()) + " bytes for " + std::to_string(want.size()) +
           ", the first difference at " + std::to_string(at);
}

// A JSON string (UTF-8) as the Latin-1 bytes a package holds; nothing when
// it has a character Latin-1 lacks.
std::optional<std::string> latin1(const std::string &utf8) {
    std::string out;
    for (std::size_t i = 0; i < utf8.size(); ++i) {
        const unsigned char c = static_cast<unsigned char>(utf8[i]);
        if (c < 0x80u) {
            out.push_back(static_cast<char>(c));
        } else if ((c == 0xC2u || c == 0xC3u) && i + 1u < utf8.size()) {
            const unsigned char next = static_cast<unsigned char>(utf8[++i]);
            out.push_back(static_cast<char>(static_cast<unsigned char>((c & 0x03u) << 6u | (next & 0x3Fu))));
        } else {
            return std::nullopt;
        }
    }
    return out;
}

const std::string *text_member(const json::Value &step, const char *name) {
    const json::Value *value = step.find(name);
    return value != nullptr && value->is(json::Value::Kind::String) ? &value->text : nullptr;
}

void round_trips(const std::filesystem::path &folder) {
    std::vector<std::filesystem::path> files;
    std::error_code error;
    for (const auto &entry : std::filesystem::directory_iterator(folder, error))
        if (entry.path().extension() == ".fep") files.push_back(entry.path());
    std::sort(files.begin(), files.end());
    if (files.empty()) report(false, "no .fep files in " + folder.string());
    for (const std::filesystem::path &path : files) {
        const std::string what = "round trip " + path.filename().string();
        const std::optional<Bytes> bytes = read_file(path);
        if (!bytes) {
            report(false, what + ": cannot read it");
            continue;
        }
        try {
            const Bytes written = fep::Package(*bytes).to_bytes();
            report(written == *bytes, what + difference(written, *bytes));
        } catch (const fep::Error &refused) {
            report(false, what + ": " + refused.what());
        }
    }
}

void copies(const std::filesystem::path &folder) {
    const std::optional<Bytes> steps_file = read_file(folder / "steps.json");
    const std::optional<Bytes> base_file = read_file(folder / "base.fep");
    const std::optional<Bytes> after_file = read_file(folder / "after.fep");
    if (!steps_file || !base_file || !after_file) {
        report(false, "steps.json, base.fep and after.fep: cannot read them");
        return;
    }
    std::string error;
    const std::optional<json::Value> steps =
        json::parse(std::string_view(reinterpret_cast<const char *>(steps_file->data()), steps_file->size()), error);
    if (!steps || !steps->is(json::Value::Kind::Array)) {
        report(false, "steps.json: " + (error.empty() ? std::string("not a list") : error));
        return;
    }

    fep::Package target(*base_file);
    std::vector<std::string> expected_names = target.names();
    std::map<std::string, fep::Package> sources;
    std::chrono::steady_clock::duration spent{};
    int number = 0;
    for (const json::Value &step : steps->items) {
        ++number;
        const std::string *source = text_member(step, "source");
        const std::string *source_name = text_member(step, "source_name");
        const std::string *name = text_member(step, "name");
        const json::Value *index = step.find("index");
        std::string what = "copy " + std::to_string(number);
        if (source == nullptr || source_name == nullptr || name == nullptr || index == nullptr ||
            !index->is(json::Value::Kind::Number)) {
            report(false, what + ": the step lacks \"source\", \"source_name\", \"name\" or \"index\"");
            continue;
        }
        what += ": " + *source + " " + *source_name + " as " + *name;
        const std::optional<std::string> old_name = latin1(*source_name);
        const std::optional<std::string> new_name = latin1(*name);
        if (!old_name || !new_name) {
            report(false, what + ": a name is not Latin-1");
            continue;
        }
        try {
            if (sources.find(*source) == sources.end()) {
                const std::optional<Bytes> bytes = read_file(folder / (*source + ".fep"));
                if (!bytes) {
                    report(false, what + ": cannot read " + *source + ".fep");
                    continue;
                }
                sources.emplace(*source, fep::Package(*bytes));
            }
            const auto start = std::chrono::steady_clock::now();
            const std::size_t got = fep::copy_effect(target, sources.at(*source), *old_name, *new_name);
            spent += std::chrono::steady_clock::now() - start;
            expected_names.push_back(*new_name);
            const std::vector<std::string> names = target.names();
            if (static_cast<double>(got) != index->number)
                report(false, what + ": index " + std::to_string(got) + " for " +
                                  std::to_string(static_cast<long long>(index->number)));
            else if (got >= names.size() || names[got] != *new_name)
                report(false, what + ": index " + std::to_string(got) + " does not hold the name");
            else
                report(true, what + " -> " + std::to_string(got));
        } catch (const fep::Error &refused) {
            report(false, what + ": " + refused.what());
        }
    }
    report(target.names() == expected_names,
           "names after the copies: " + std::to_string(target.names().size()) + ", base.fep's and then each copy's");
    Bytes written;
    try {
        written = target.to_bytes();
        report(written == *after_file, "after.fep" + difference(written, *after_file));
    } catch (const fep::Error &refused) {
        report(false, std::string("after.fep: ") + refused.what());
    }
    try {
        report(fep::Package(written).names() == expected_names, "names of the package written, read back");
    } catch (const fep::Error &refused) {
        report(false, std::string("names of the package written, read back: ") + refused.what());
    }
    const long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(spent).count();
    std::printf("     %d copies took %lld ms\n", number, ms);
}

} // namespace

int main(int argc, char **argv) {
    if (argc != 2) {
        std::fprintf(stderr, "usage: check_effect_package <dev dir>   (the folder that holds ref/fep)\n");
        return 2;
    }
    const std::filesystem::path folder = std::filesystem::path(argv[1]) / "ref" / "fep";
    try {
        round_trips(folder);
        copies(folder);
    } catch (const std::exception &failure) {
        report(false, std::string("stopped: ") + failure.what());
    }
    if (g_failed == 0) std::printf("all %d checks passed\n", g_checks);
    else std::printf("%d of %d checks FAILED\n", g_failed, g_checks);
    return g_failed == 0 ? 0 : 1;
}
