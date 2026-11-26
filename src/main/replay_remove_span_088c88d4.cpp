/* Camera Edit: remove the span entry at a frame: a plain entry (kind 0) there, else the start and end markers around that frame, else the first plain entry. */
#include "main/replay_types.h"
extern "C" D012ReplayCameraEntry *func_088C9044(D012Replay *replay, unsigned int n);
extern "C" int func_088C8CC0(D012Replay *replay, unsigned int n);

extern "C" int func_088C88D4(D012Replay *replay, int frame)
{
    for (int i = 0; i < replay->span_count; i++) {
        D012ReplayCameraEntry *entry = func_088C9044(replay, i);
        if (entry == 0) return 0;
        if (entry->kind != 0) continue;
        if (entry->frame == frame) {
            func_088C8CC0(replay, i);
            return 1;
        }
        break;
    }
    for (int j = 0; j < replay->span_count; j++) {
        D012ReplayCameraEntry *entry = func_088C9044(replay, j);
        if (entry == 0) return 0;
        if (entry->kind != 1) continue;
        unsigned int end = 0xFFFFFFFF;
        int endFrame = 0;
        for (int k = j + 1; k < replay->span_count; k++) {
            D012ReplayCameraEntry *e = func_088C9044(replay, k);
            if (e == 0) return 0;
            if (e->kind != 2) continue;
            endFrame = e->frame;
            end = k;
            break;
        }
        if (end >= (unsigned int)replay->span_count) break;
        if (frame < entry->frame || endFrame < frame) continue;
        func_088C8CC0(replay, end);
        func_088C8CC0(replay, j);
        return 1;
    }
    for (int m = 0; m < replay->span_count; m++) {
        D012ReplayCameraEntry *entry = func_088C9044(replay, m);
        if (entry == 0) return 0;
        if (entry->kind != 0) continue;
        func_088C8CC0(replay, m);
        return 1;
    }
    return 0;
}
