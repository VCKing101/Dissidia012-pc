/* The replay recorder (called by the replay task once a frame unless a replay plays): each fighter's 15 pad bits
   and its "pad changed" bit as runs of up to 255 frames, a camera entry when the camera event bit is set, and the
   frame clock. See docs/research/replay-module.md section 3.2. */
#include "main/replay_types.h"
/* SNC virtual calls: each vtable entry is {this adjustment, function}. A fighter's pad (vtable at +0x34): entry 6
   says whether the pad is active; its raw pad (+0x40, vtable at +0x34): entry 1 tests a 64-bit button mask. */
typedef struct PadPmf { short adj; short pad; int (*fn)(void *); } PadPmf;
typedef struct HeldPmf { short adj; short pad; int (*fn)(void *, unsigned long long); } HeldPmf;
typedef struct RawVt { char p0[0x8]; HeldPmf held; } RawVt;
typedef struct RawPad { char p0[0x34]; RawVt *vt; } RawPad;
typedef struct PadVt { char p0[0x30]; PadPmf active; } PadVt;
typedef struct Pad { char p0[0x34]; PadVt *vt; char p1[0x8]; RawPad *raw; } Pad;
extern "C" unsigned long long D_089D7758[15];
extern "C" unsigned int func_088C8434(D012Replay *replay);
extern "C" void func_088CA564(D012Replay *replay, int fighter);

extern "C" void func_088CA680(D012Replay *replay, Pad **pads, unsigned char *pad_changed, unsigned char camera_event)
{
    int f; /* a counter of its own for the full check: sharing i changes the register allocation */
    int i;
    switch (replay->state) {
    case 0:
        replay->state = 1;
        replay->frame_total = 0;
    case 1:
        for (f = 0; f < 2; f++) {
            if (replay->record_count[f] >= D012_REPLAY_SLOTS) {
                replay->flags |= D012_REPLAY_FULL;
                break;
            }
        }
        for (i = 0; i < 2; i++) {
            Pad *pad = pads[i];
            if (pad != 0) {
                unsigned int mask = 0;
                PadPmf *active = &pad->vt->active;
                if (active->fn((char *)pad + active->adj) == 1) {
                    RawPad *raw = pad->raw;
                    unsigned long long *bits = D_089D7758;
                    int bit;
                    for (bit = 0; bit < 15; bit++) {
                        HeldPmf *held = &raw->vt->held;
                        if (held->fn((char *)raw + held->adj, *bits) != 0) {
                            mask |= 1 << bit;
                        }
                        bits++;
                    }
                }
                if (pad_changed[i] == 1) {
                    mask |= 0x8000;
                }
                if (mask != replay->rec_last[i] || replay->rec_run[i] >= 0xFF) {
                    func_088CA564(replay, i);
                    replay->rec_last[i] = mask;
                    replay->rec_run[i] = 1;
                } else {
                    replay->rec_run[i]++;
                }
            }
        }
        if (camera_event == 1) {
            unsigned int index = func_088C8434(replay);
            if (index < D012_REPLAY_CAMERA_SLOTS) {
                D012ReplayCameraEntry cam;
                cam.unk_1C = cam.unk_1E = cam.kind = cam.unk_21[0] = cam.unk_21[1] = cam.unk_21[2] = 0;
                cam.frame = replay->frame_total;
                *(D012ReplayCameraEntry *)&replay->camera[index] = cam;
                replay->camera_count++;
            } else {
                replay->flags |= D012_REPLAY_FULL;
            }
        }
        replay->frame_total++;
        break;
    case 3: /* recording stopped */
        break;
    default:
        return;
    }
}
