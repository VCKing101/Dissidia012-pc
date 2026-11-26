/* Camera Edit: delete span entry n by moving every later entry down one place, then count one fewer. */
#include "main/replay_types.h"
extern "C" D012ReplayCameraEntry *func_088C9044(D012Replay *replay, unsigned int n);

extern "C" int func_088C8CC0(D012Replay *replay, unsigned int n)
{
    int last;
    if (n >= (unsigned int)replay->span_count) {
        return 0;
    }
    for (int j = n; j < (last = replay->span_count - 1); ) {
        int i = j;
        D012ReplayCameraEntry *dst = func_088C9044(replay, j);
        i++;
        D012ReplayCameraEntry *src = func_088C9044(replay, i);
        if (dst == 0 || src == 0) {
            return 0;
        }
        *dst = *src;
        j = i;
    }
    replay->span_count = last;
    return 1;
}
