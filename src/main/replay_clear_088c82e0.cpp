/* Clears the replay object's saved part: the format word 0x20081106, the counts, both record arrays, the 200
   camera entries and the six words after them; then 0x088FF7B8. See docs/research/replay-module.md. */
#include "main/replay_types.h"
extern "C" void func_08935828(void *dst, int value, unsigned int size);
extern "C" void func_088FF7B8(void);

extern "C" void func_088C82E0(D012ReplayData *replay)
{
    replay->magic = D012_REPLAY_MAGIC;
    for (int i = 0; i < 2; i++) {
        replay->record_count[i] = 0;
    }
    func_08935828(replay->records, 0, sizeof(replay->records));
    for (int i = 0; i < 200; i++) {
        replay->camera[i].Clear();
    }
    replay->camera_data_count = 0;
    replay->camera_count = 0;
    replay->span_count = 0;
    replay->frame_total = 0;
    replay->unk_BDB0 = 0;
    replay->player_side = 0;
    func_088FF7B8();
}
