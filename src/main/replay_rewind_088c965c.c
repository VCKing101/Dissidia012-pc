/* Rewinds the replay object for playing it again: state 0, the player's and recorder's cursors and the
   play frame zeroed; the data and the flags are kept. */
#include "main/replay_types.h"

void func_088C965C(D012Replay *replay)
{
    int i;
    int j;
    replay->state = 0;
    for (i = 0; i < 2; i++) {
        replay->play_mask[i] = 0;
        replay->play_left[i] = 0;
        replay->play_index[i] = 0;
    }
    for (j = 0; j < 2; j++) {
        replay->rec_last[j] = 0;
        replay->rec_run[j] = 0;
    }
    replay->play_frame = 0;
}
