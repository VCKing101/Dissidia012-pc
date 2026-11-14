// What the player is told when the game cannot open its window; see
// d012_no_window_rules.hpp.
// MIT licence; see port/README.md.

#include "d012_no_window_rules.hpp"

#include <string_view>

namespace d012::no_window_rules {

std::string message(const std::string &why, System system, const std::string &log) {
    std::string text = "Dissidia 012 can't start its graphics, so it can't open its window.\n\nReason: " + why + "\n\n";
    switch (system) {
    case System::Windows:
        text += "The game draws with Vulkan, which comes with your graphics driver. Install the current driver for "
                "your graphics card from AMD, NVIDIA or Intel (again, if you have it already), then start the game "
                "again.";
        break;
    case System::Linux:
        text += "The game draws with Vulkan. Install your graphics driver's Vulkan support (Mesa's Vulkan drivers, "
                "or NVIDIA's own driver), then start the game again.";
        break;
    case System::Other:
        text += "The game draws with Vulkan. Update your graphics driver, then start the game again.";
        break;
    }
    if (!log.empty()) text += "\n\nFor a bug report, the log is:\n" + log;
    return text;
}

bool keep_running(const char *value) {
    return value != nullptr && *value != '\0' && std::string_view(value) != "0";
}

} // namespace d012::no_window_rules
