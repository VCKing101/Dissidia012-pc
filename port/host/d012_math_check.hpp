#pragma once

// D012_MATH_CHECK=1, or a copy set up for network battles (d012_net.hpp;
// D012_MATH_CHECK=0 keeps it quiet): prints, once at start-up, a fingerprint
// of the library maths the recompiled game reaches (sine, cosine, arcsine,
// log2 and exp2 of a float, from the VFPU's vsin/vcos/vrot/vasin/vlog2/vexp2),
// over a few million inputs. Two copies that link for a battle must get the same bits
// from these; the same .exe on Windows and under Proton may not, since each
// takes them from its own C runtime. Compare the lines of the two machines.
// MIT licence; see port/README.md.

namespace d012::math_check {

void run_if_asked();

} // namespace d012::math_check
