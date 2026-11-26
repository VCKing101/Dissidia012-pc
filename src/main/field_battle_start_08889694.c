/* A battle's start (also run by 0x0888964C when the mode is set): resets FIELD's per-battle state and the pad
   change trackers, then the replay object: in mode 9 a full reset when the replay plays with its camera data or
   this is not a restart, else a rewind and the stored set-up applied again; otherwise a full reset for recording
   and the player's side noted. See docs/research/replay-module.md section 6. */
#include "main/replay_types.h"
typedef struct Field {
    int unk_00;
    char unk_04[0x4C - 0x4];
    int mode;
    int stage_state;
    char p0[0x58 - 0x54];
    int unk_58;
    int unk_5C;
    float unk_60;
    int retries;
    unsigned char restart;
    unsigned char unk_69;
    char p1[0x73C - 0x6A];
    int unk_73C;
    char p2[0x7A4 - 0x740];
    int unk_7A4;
    char p3[0x7B8 - 0x7A8];
    int map_kind_prev;
    int map_kind;
    unsigned char pad_state_prev[2];
    unsigned char pad_state[2];
    unsigned char camera_event_prev;
    char p4[3];
    D012Replay *replay;
} Field;
typedef struct FieldSide { char p0[0x54]; int side; } FieldSide;
extern FieldSide *D_089D6A10;
extern void func_0886503C(void *list, void *a, void *b, void *c);
extern void func_08889098(void);
extern void func_088890B8(void);
extern void func_088890D8(void);
extern void func_088C95A0(D012Replay *replay);
extern void func_088C965C(D012Replay *replay);
extern void func_088C98F0(D012Replay *replay);
extern void func_088C9928(D012Replay *replay, int side);

void func_08889694(Field *field, unsigned char restart)
{
    D012Replay *replay;
    int i;
    field->unk_58 = 1;
    field->unk_5C = 1;
    func_0886503C(field->unk_04, func_08889098, func_088890B8, func_088890D8);
    replay = field->replay;
    if (restart != 0 || field->stage_state == 4) {
        field->retries++;
    } else if (field->stage_state != 3) {
        field->retries = 0;
    }
    field->unk_7A4 = 0;
    field->stage_state = 2;
    field->unk_60 = 0.0f;
    field->restart = restart;
    field->unk_69 = 0;
    field->unk_73C = 0;
    field->map_kind_prev = 1;
    field->map_kind = 1;
    for (i = 0; i < 2; i++) {
        field->pad_state_prev[i] = 1;
        field->pad_state[i] = 1;
    }
    field->camera_event_prev = 0;
    if (field->mode == 9) {
        int full = 1;
        if (!(unsigned char)((replay->flags & D012_REPLAY_CAMERA) != 0) && field->restart != 0) {
            full = 0;
        }
        if (full) {
            func_088C95A0(replay);
        } else {
            func_088C965C(replay);
            func_088C98F0(field->replay);
        }
    } else {
        func_088C95A0(replay);
        func_088C9928(field->replay, D_089D6A10->side);
    }
}
