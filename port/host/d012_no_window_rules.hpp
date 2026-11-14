#pragma once

// What the player is told when the game cannot open its window
// (d012_no_window.cpp): no window here, so d012_tests checks it.
// MIT licence; see port/README.md.

#include <string>

namespace d012::no_window_rules {

enum class System { Windows, Linux, Other };

// The message box's text: what happened, the renderer's reason (`why`), what
// to do about it on `system`, and where the log is (`log`, UTF-8; empty when
// the output goes to no file of the program's own, and the line is left out).
[[nodiscard]] std::string message(const std::string &why, System system, const std::string &log);

// Whether D012_HEADLESS_FALLBACK's value (null when it is not set) asks the
// game to run on without a window, as it did before: set, and not "0".
[[nodiscard]] bool keep_running(const char *value);

} // namespace d012::no_window_rules
