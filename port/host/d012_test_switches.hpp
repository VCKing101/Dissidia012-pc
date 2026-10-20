#pragma once

// Test switches (d012_test_switches.cpp): D012_TEST_UNLOCK_FIGHTERS and
// D012_TEST_EX_FULL, off by default.

namespace portablekit {
class HleRegistrar;
}

namespace d012::test_switches {

void install(portablekit::HleRegistrar &hle);

} // namespace d012::test_switches
