/* Camera Edit: normalize the span range between the last open entry's frame and the given frame, replacing the entries inside it with a start and an end marker. */
#include "main/replay_types.h"
extern "C" D012ReplayCameraEntry *func_088C9044(D012Replay *replay, unsigned int n);
extern "C" int func_088C8B90(D012Replay *replay, int frame, unsigned int kind);
extern "C" int func_088C8CC0(D012Replay *replay, unsigned int n);
extern "C" int func_088C83FC(D012Replay *replay);

extern "C" int func_088C854C(D012Replay *replay, int frame)
{
    int hasStart, hasEnd;
    unsigned int found = 0xFFFFFFFF;
    for (int i = 0; i < replay->span_count; i++) {
        D012ReplayCameraEntry *entry = func_088C9044(replay, i);
        if (entry == 0) return 0;
        if (entry->kind != 0) continue;
        found = i;
        break;
    }
    if (found >= (unsigned int)replay->span_count) {
        if (!func_088C8B90(replay, frame, 0)) return 0;
    } else {
        D012ReplayCameraEntry *entry = func_088C9044(replay, found);
        if (entry == 0) return 0;
        if (entry->frame == frame) return 0;
        int ev = ((D012ReplayCamera *)entry)->frame;
        int lo = ev < frame ? ev : frame;
        int old = frame;
        int ef = ((D012ReplayCamera *)entry)->frame;
        int top = old;
        top = old < ef ? ef : top;
        unsigned int at = found;
        found = top;
        func_088C8CC0(replay, at);
        hasStart = 0;
        for (int j = 0; j < replay->span_count; j++) {
            D012ReplayCameraEntry *e = func_088C9044(replay, j);
            if (e == 0) return 0;
            if (e->frame >= lo) break;
            hasStart = e->kind == 1;
        }
        hasEnd = 0;
        for (int k = 0; k < replay->span_count; k++) {
            D012ReplayCameraEntry *e = func_088C9044(replay, k);
            if (e == 0) return 0;
            if ((int)found >= e->frame) continue;
            hasEnd = e->kind == 2;
            break;
        }
        for (unsigned int m = 0; m < (unsigned int)replay->span_count; ) {
            D012ReplayCameraEntry *e = func_088C9044(replay, m);
            if (e == 0) return 0;
            if (!(e->frame < lo || (int)found < e->frame)) {
                func_088C8CC0(replay, m);
            } else {
                m++;
            }
        }
        int room = (!hasStart) + (!hasEnd);
        if (func_088C83FC(replay) < room) return 0;
        if (!hasStart) {
            if (!func_088C8B90(replay, lo, 1)) return 0;
        }
        if (!hasEnd) {
            if (!func_088C8B90(replay, found, 2)) return 0;
        }
    }
    return 1;
}
