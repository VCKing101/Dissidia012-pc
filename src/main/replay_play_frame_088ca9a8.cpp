// Replay player: plays one frame of the recorded pad input (and camera data) back into the two fighters' pads.
#include "main/replay_types.h"
extern "C" {
extern int D_089D6008;
extern unsigned long long D_089D7758[];
extern unsigned long long D_089CBC70;
void func_088FF7CC(void);
void func_088FF868(void);
void func_088FF8F8(int on);
int func_088CA540(D012Replay *replay);
void func_088CA664(unsigned int code, unsigned int *high, unsigned int *low);
void func_08872970(void *pad, unsigned long long buttons);
unsigned long long func_08871BC4(void *pad);
void func_08871BD0(void *pad, unsigned long long v);
}
// A second struct type for the flags word: SNC then keeps the play_frame store before the flags load (it may alias).
// The test is a member function: a static helper would be emitted on its own.
struct ReplayFlagsView {
    unsigned char pad[0xBDE8];
    unsigned int flags;
    bool has_camera() { return (flags & D012_REPLAY_CAMERA) != 0; }
};
extern "C" void func_088CA9A8(D012Replay *replay, void **pads)
{
    switch (replay->state) {
    case 0:
        replay->state = 2;
        *(unsigned int *)(D_089D6008 + 0x264) |= 1;
        for (int i = 0; i < 2; i++) {
            replay->play_mask[i] = 0;
            replay->play_left[i] = 0;
            replay->play_index[i] = 0;
        }
        replay->play_frame = 0;
        if (((ReplayFlagsView *)replay)->has_camera()) {
            func_088FF7CC();
            func_088FF868();
        }
        /* falls into case 2 */
    case 2:
        if (((ReplayFlagsView *)replay)->has_camera()) {
            if (func_088CA540(replay) != 0) func_088FF8F8(1);
            else func_088FF8F8(0);
        }
        for (int i = 0; i < 2; i++) {
            if (replay->play_left[i] <= 0 && replay->play_index[i] >= replay->record_count[i]) continue;
            {
                if (replay->play_left[i] <= 0) {
                    int code = 0;
                    int off = replay->play_index[i] * 3;
                    for (int k = 0; k < 3; k++) {
                        code = code * 0x100;
                        code = code + replay->records[i][off + k];
                    }
                    unsigned int mask = 0, run = 0;
                    func_088CA664(code, &mask, &run);
                    replay->play_mask[i] = mask;
                    replay->play_left[i] = run;
                    replay->play_index[i]++;
                }
                int left = replay->play_left[i] - 1;
                unsigned long long base = D_089CBC70;
                replay->play_left[i] = left;
                unsigned long long buttons = base;
                for (int b = 0; b < 15; b++) {
                    if (replay->play_mask[i] & (1 << b))
                        buttons |= D_089D7758[b];
                }
                void *pad = pads[i];
                func_08872970(pad, buttons);
                if (replay->play_mask[i] & 0x8000) {
                    func_08871BD0(pad, func_08871BC4(pad));
                }
            }
        }
        replay->play_frame++;
        break;
    case 4:
        replay->state = 5;
        break;
    case 5:
        break;
    case 6:
        break;
    default:
        break;
    }
}
