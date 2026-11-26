/* Camera Edit: open a slot in the span entries at an index (shift later entries up one place), fill in its frame and kind. */
#include "main/replay_types.h"
extern "C" D012ReplayCameraEntry *func_088C9044(D012Replay *replay, unsigned int n);
extern "C" int func_088C83FC(D012Replay *replay);

extern "C" int func_088C8DE8(D012Replay *replay, int index, int frame, unsigned int kind)
{
    if (kind >= 3) {
        return 0;
    }
    if (func_088C83FC(replay) > 0) {
        int j = replay->span_count;
        replay->span_count = j + 1;
        while (index < j) {
            int i = j;
            D012ReplayCameraEntry *dst = func_088C9044(replay, i);
            i--;
            D012ReplayCameraEntry *src = func_088C9044(replay, i);
            if (dst == 0 || src == 0) {
                return 0;
            }
            *dst = *src;
            j = i;
        }
    } else {
        return 0;
    }
    D012ReplayCameraEntry *entry = func_088C9044(replay, index);
    if (entry == 0) {
        return 0;
    }
    entry->frame = frame;
    entry->kind = kind;
    return 1;
}
