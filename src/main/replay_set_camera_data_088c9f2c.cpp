/* Camera Edit: store a camera data entry over the one found at the cursor (keeping its frame), or add it as a new entry. */
#include "main/replay_types.h"
extern "C" D012ReplayCameraEntry *func_088CA240(D012Replay *replay);
extern "C" int func_088C9AEC(D012Replay *replay, D012ReplayCameraEntry *e);
extern "C" int func_088C9F2C(D012Replay *replay, D012ReplayCameraEntry *e)
{
    D012ReplayCameraEntry *dst = func_088CA240(replay);
    if (dst) { int frame = dst->frame; *dst = *e; dst->frame = frame; } else { func_088C9AEC(replay, e); }
    return 1;
}
