#pragma once

// Save states for rollback netplay, and their test (docs/port/online-next.md,
// section 5). D012_ROLLBACK_TEST=K[,EVERY[,wrong]] saves the whole state in
// battle at every EVERY-th pad read (60 by default), runs K more reads, puts
// the state back and runs those K reads again. With "wrong" the first run of
// them gets player 2's input wrong (another stick and button, as a
// misprediction would), and only the second run the right input. With D012_INPUT_REPLAY the second run
// gets the same input, so a run's D012_SYNC_TRACE must match a run without the
// test read for read (tools/port/sync_compare.py, or the scratchpad's own):
// each page that differs is state a save missed.

#include "psprecomp/runtime.hpp"

#include <cstdint>

namespace d012::rollback {

// Adds the port's own state to save states and reads the test switch.
void install();

// After each pad read (d012_versus.cpp), with the trace's read number and
// whether a battle is on.
void note_pad_read(std::uint64_t read, bool battle);

// Between two dispatches (the harness's post-dispatch hook): where a save or
// a restore the last pad read asked for happens.
void after_dispatch(psprecomp::Runtime &rt);

// The test's misprediction: true while player 2's input should be wrong.
[[nodiscard]] bool wrong_input();

} // namespace d012::rollback
