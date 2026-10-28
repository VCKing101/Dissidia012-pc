// Checking the mod being edited; see checks.hpp. MIT licence; see
// port/README.md.

#include "checks.hpp"

namespace d012::bgm_maker {

namespace {

void check_others(std::vector<Problem> &problems, const std::string &id, const std::vector<ModSummary> &others) {
    if (id.empty()) return;
    for (const ModSummary &other : others) {
        if (other.id != id) continue;
        problems.push_back({Problem::Level::Error, "mod.id",
                            "the folder \"" + other.folder + "\" already has the id \"" + id +
                                "\"; the port would leave one of them out",
                            0});
    }
}

} // namespace

std::vector<Problem> check_mod(const ModModel &model, const std::filesystem::path &folder,
                               const std::vector<ModSummary> &others) {
    std::vector<Problem> problems = mod_manifest::read_text(manifest_text(model), folder).problems;
    // The line numbers are in a manifest that is not on disc yet; paths say where.
    for (Problem &problem : problems) problem.line = 0;
    check_others(problems, model.id, others);
    return problems;
}

std::vector<Problem> check_text(const std::string &text, const std::string &id, const std::filesystem::path &folder,
                                const std::vector<ModSummary> &others) {
    std::vector<Problem> problems = mod_manifest::read_text(text, folder).problems;
    check_others(problems, id, others);
    return problems;
}

std::string folder_name_problem(std::string_view name, const std::filesystem::path &mods) {
    if (name.empty()) return "the folder needs a name";
    if (name == "." || name == ".." || name.front() == '.') return "the folder's name may not start with '.'";
    for (const char c : name)
        if (c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|' ||
            static_cast<unsigned char>(c) < 0x20u)
            return "the folder's name may not contain / \\ : * ? \" < > |";
    std::error_code ec;
    if (std::filesystem::exists(mods / mod_manifest::path_from_utf8(name), ec))
        return "the mods folder already has \"" + std::string(name) + "\"";
    return {};
}

std::vector<const Problem *> problems_at(const std::vector<Problem> &problems, std::string_view path, bool below) {
    std::vector<const Problem *> found;
    for (const Problem &problem : problems) {
        const std::string_view at = problem.path;
        if (at == path || (below && at.size() > path.size() && at.starts_with(path) && at[path.size()] == '.'))
            found.push_back(&problem);
    }
    return found;
}

} // namespace d012::bgm_maker
