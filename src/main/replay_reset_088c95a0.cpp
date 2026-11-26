/* Resets the replay object for a new recording: state 0, everything saved cleared (0x088C82E0), the player's and
   recorder's cursors, the play frame and the flags zeroed. */
#include "main/replay_types.h"
extern "C" void func_088C82E0(D012Replay *replay);

extern "C" void func_088C95A0(D012Replay *replay)
{
    replay->state = 0;
    func_088C82E0(replay);
    for (int i = 0; i < 2; i++) {
        replay->play_mask[i] = 0;
        replay->play_left[i] = 0;
        replay->play_index[i] = 0;
    }
    for (int j = 0; j < 2; j++) {
        replay->rec_last[j] = 0;
        replay->rec_run[j] = 0;
    }
    replay->play_frame = 0;
    replay->flags = 0;
}
