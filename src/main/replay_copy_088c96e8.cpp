/* Copies a loaded replay into the replay object member by member (the load path, from main 0x08889BAC), then
   applies its set-up to the battle. See docs/research/replay-module.md section 6. */
#include "main/replay_types.h"
extern "C" void func_088FB028(D012ReplaySetup *setup);

extern "C" void func_088C96E8(D012ReplayData *replay, D012ReplayData *loaded)
{
    *replay = *loaded;
    func_088FB028(&replay->setup);
}
