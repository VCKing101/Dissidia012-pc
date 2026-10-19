#pragma once

// Dissidia 012's own additions to PortableKit's HLE and loaded image, reached
// through the profile's two hooks (host/profile.hpp).

namespace psprecomp {
class Elf32Image;
class Runtime;
} // namespace psprecomp

namespace portablekit {
class HleRegistrar;
}

namespace d012 {

void register_extra_hle(portablekit::HleRegistrar &hle);
// LIBSUPPREACC.PRX loaded for real, its exports serving main's imports
// (d012_prx.cpp).
void register_suppreacc(portablekit::HleRegistrar &hle);
void patch_loaded_image(psprecomp::Runtime &runtime, const psprecomp::Elf32Image &elf);

} // namespace d012
