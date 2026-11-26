/* Camera Edit spans by frame: the replay's length less the edited spans (0x088C93AC, at least 0), and whether a
   frame lies inside a span (0x088C948C; the player passes the answer to 0x088FF8F8 each frame). */
#include "main/replay_types.h"
extern "C" int func_088C913C(D012Replay *replay);
extern "C" D012ReplayCamera *func_088C91EC(D012Replay *replay, int n);
extern "C" D012ReplayCamera *func_088C92CC(D012Replay *replay, int n);

extern "C" int func_088C93AC(D012Replay *replay)
{
    int skipped = 0;
    int spans = func_088C913C(replay);
    for (int i = 0; i < spans; i++) {
        D012ReplayCamera *start = func_088C91EC(replay, i);
        D012ReplayCamera *end = func_088C92CC(replay, i);
        if (start == 0 || end == 0) {
            break;
        }
        skipped += end->frame - start->frame;
    }
    int length = replay->frame_total - skipped;
    if (length < 0) {
        length = 0;
    }
    return length;
}

extern "C" int func_088C948C(D012Replay *replay, int frame)
{
    int inside = 0;
    int spans = func_088C913C(replay);
    for (int i = 0; i < spans; i++) {
        D012ReplayCamera *start = func_088C91EC(replay, i);
        if (start != 0 && frame < start->frame) {
            break;
        }
        D012ReplayCamera *end = func_088C92CC(replay, i);
        if (end != 0 && frame < end->frame) {
            inside = 1;
            break;
        }
    }
    return inside;
}
