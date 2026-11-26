/* Camera Edit spans: the n-th edit entry that starts a span (kind 1) and the n-th that ends one (kind 2), or 0.
   Edit entries are counted at replay +0xBDA8 and reached through 0x088C90C0. */
#include "main/replay_types.h"
extern "C" D012ReplayCamera *func_088C90C0(D012Replay *replay, unsigned int index);
extern "C" int func_088C913C(D012Replay *replay);

extern "C" D012ReplayCamera *func_088C91EC(D012Replay *replay, int n)
{
    D012ReplayCamera *result = 0;
    int found = 0;
    for (int i = 0; i < replay->span_count; i++) {
        D012ReplayCamera *entry = func_088C90C0(replay, i);
        if (entry == 0) continue;
        if (entry->kind != 1) continue;
        if (found == n) {
            result = entry;
            break;
        }
        found++;
    }
    return result;
}


extern "C" D012ReplayCamera *func_088C92CC(D012Replay *replay, int n)
{
    D012ReplayCamera *result = 0;
    int found = 0;
    for (int i = 0; i < replay->span_count; i++) {
        D012ReplayCamera *entry = func_088C90C0(replay, i);
        if (entry == 0) continue;
        if (entry->kind != 2) continue;
        if (found == n) {
            result = entry;
            break;
        }
        found++;
    }
    return result;
}

