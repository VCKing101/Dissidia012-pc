/* Camera Edit: add a span entry of the given kind (0..2) at a frame, in frame order: before the first entry at or after that frame, or at the end. */
#include "main/replay_types.h"
extern "C" D012ReplayCameraEntry *func_088C9044(D012Replay *replay, unsigned int n);
extern "C" int func_088C8DE8(D012Replay *replay, unsigned int index, int frame, unsigned int kind);
extern "C" int func_088C8F64(D012Replay *replay, int frame, unsigned int kind);

extern "C" int func_088C8B90(D012Replay *replay, int frame, unsigned int kind)
{
    if (kind >= 3) {
        return 0;
    }
    int found = -1;
    for (int i = 0; i < replay->span_count; i++) {
        D012ReplayCameraEntry *entry = func_088C9044(replay, i);
        if (entry == 0) {
            return 0;
        }
        if (entry->frame < frame) {
            continue;
        }
        found = i;
        break;
    }
    if ((unsigned int)found < (unsigned int)replay->span_count) {
        if (!func_088C8DE8(replay, found, frame, kind)) {
            return 0;
        }
    } else {
        if (!func_088C8F64(replay, frame, kind)) {
            return 0;
        }
    }
    return 1;
}
