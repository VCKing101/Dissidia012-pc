// Checks the port's builder of Bartz's original moves (host/d012_original_moves.cpp)
// against the Python one (tools/original_moves.py), with the game's files
// that tools/original_moves_refs.py wrote:
//
//     check_original_moves <dev dir> <recipe json>
//
// <dev dir>/disc holds the disc's files by package path and
// <dev dir>/expect/package the files the Python build made. The files are the
// game's, so this is a developer's check and no part of the test run. Exit
// code 0: the inputs are the recipe's, and every file came out byte for byte
// as Python's and as the recipe's check block says. MIT licence; see
// port/README.md.

#include "d012_json.hpp"
#include "d012_original_moves.hpp"

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <string>
#include <vector>

namespace {

std::optional<std::vector<std::uint8_t>> read_file(const std::filesystem::path &path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return std::nullopt;
    return std::vector<std::uint8_t>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

} // namespace

int main(int argc, char **argv) {
    namespace om = d012::original_moves;
    if (argc != 3) {
        std::cerr << "usage: check_original_moves <dev dir> <recipe json>\n";
        return 2;
    }
    const std::filesystem::path dev = argv[1];
    const auto recipe_bytes = read_file(argv[2]);
    if (!recipe_bytes) {
        std::cerr << argv[2] << " cannot be read\n";
        return 2;
    }
    std::string error;
    const auto recipe = d012::json::parse(std::string_view(reinterpret_cast<const char *>(recipe_bytes->data()), recipe_bytes->size()), error);
    if (!recipe) {
        std::cerr << "the recipe: " << error << "\n";
        return 2;
    }
    bool good = true;
    try {
        for (const om::Mark &mark : om::inputs(*recipe)) {
            const auto bytes = read_file(dev / "disc" / mark.path);
            const bool same = bytes && bytes->size() == mark.size && om::crc32(*bytes) == mark.crc;
            if (!same) std::cout << "input " << mark.path << ": not the recipe's\n";
            good = good && same;
        }
        const auto started = std::chrono::steady_clock::now();
        const om::Built built = om::build(*recipe, [&](std::string_view path) { return read_file(dev / "disc" / path); });
        const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count();
        for (const std::string &note : built.notes) std::cout << "note: " << note << "\n";
        for (const om::File &file : built.files) {
            const auto expected = read_file(dev / "expect" / "package" / file.path);
            std::string result = "same";
            if (!expected) {
                result = "Python made no such file";
            } else if (*expected != file.bytes) {
                std::size_t at = 0u;
                while (at < expected->size() && at < file.bytes.size() && (*expected)[at] == file.bytes[at]) ++at;
                result = "DIFFERS at byte " + std::to_string(at) + " (Python's is " + std::to_string(expected->size()) + " bytes)";
            }
            good = good && result == "same";
            char crc[9];
            std::snprintf(crc, sizeof crc, "%08X", static_cast<unsigned>(om::crc32(file.bytes)));
            std::cout << file.path << ": " << file.bytes.size() << " bytes, CRC " << crc << ", " << result << "\n";
        }
        for (const auto &[id, level] : built.abilities) std::printf("ability %04X at level %d\n", static_cast<unsigned>(id), level);
        std::string problem;
        if (!om::check(*recipe, built, problem)) {
            std::cout << "the recipe's check: " << problem << "\n";
            good = false;
        } else {
            std::cout << "the recipe's check: passed\n";
        }
        std::printf("built in %.3f s\n", seconds);
    } catch (const om::Error &failure) {
        std::cout << "the build failed: " << failure.what() << "\n";
        return 1;
    }
    std::cout << (good ? "all the same\n" : "NOT the same\n");
    return good ? 0 : 1;
}
