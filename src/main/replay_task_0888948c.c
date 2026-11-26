/* "FIELD::ReplayRecordAndPlayTask" (FIELD task list, filter 3, priority 300): in a battle's phases 2 to 4, gives
   the replay player (mode 9) or the recorder both fighters' pads, once a frame. While a replay plays with its
   camera data, it also starts the battle music when FIELD +0x7C5 counts down. See
   docs/research/replay-module.md section 3.1. */
#include "main/replay_types.h"
typedef struct Fighter { char p0[0x5A0]; void *pad; } Fighter;
typedef struct Field {
    char p0[0x38];
    Fighter *fighters[2];
    char p1[0x48 - 0x40];
    int phase;
    int mode;
    char p2[0x7B8 - 0x50];
    int map_kind_prev;               /* set each frame by ExecPadInitByChangeTask */
    int map_kind;
    unsigned char pad_state_prev[2];
    unsigned char pad_state[2];
    unsigned char camera_event_prev;
    signed char music_delay;
    char p3[2];
    D012Replay *replay;
    char p4[0x804 - 0x7CC];
    int bgm;
} Field;
extern Field *D_089D6A10;
extern int D_089D6C88;
extern int D_089D6C30;
extern int func_0888B0A4(void);
extern int func_08889468(Field *field, int kind);
extern void func_088A18B0(int sound, int volume, int fade);
extern void func_0889F234(int bgm_player, int bgm);
extern void func_088CA9A8(D012Replay *replay, void **pads);
extern void func_088CA680(D012Replay *replay, void **pads, unsigned char *pad_changed, unsigned char camera_event);

void func_0888948C(void)
{
    Field *field = D_089D6A10;
    void *pads[2];
    unsigned char changed[2];
    int i;
    int j;
    /* The do/while(0) with a break gives retail's branches for the phase test. */
    do {
        if (!(field->phase == 3 || field->phase == 2 || field->phase == 4)) break;
        for (i = 0; i < 2; i++) {
            pads[i] = field->fighters[i]->pad;
        }
        if (field->mode == 9) {
            if ((unsigned char)((field->replay->flags & D012_REPLAY_CAMERA) != 0) && field->music_delay > 0) {
                field->music_delay--;
                if (field->music_delay <= 0) {
                    if (field->bgm != -1) {
                        func_088A18B0(D_089D6C88, 100, 0);
                        func_0889F234(D_089D6C30, field->bgm);
                    }
                    field->music_delay = 0;
                }
            }
            func_088CA9A8(field->replay, pads);
        } else {
            int event = func_0888B0A4();
            int kind_a = func_08889468(field, field->map_kind_prev);
            int kind_b = func_08889468(field, field->map_kind);
            int kinds_differ = kind_a != kind_b;
            for (j = 0; j < 2; j++) {
                unsigned char c = 0;
                if (kinds_differ == 1 || field->pad_state_prev[j] != field->pad_state[j]) {
                    c = 1;
                }
                changed[j] = c;
            }
            func_088CA680(field->replay, pads, changed, event != field->camera_event_prev);
            field->camera_event_prev = event;
        }
    } while (0);
}
