#include "battle/agent23_types.h"

typedef struct MotionTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x8];
    char part_14[0x4];
    char pad_18[0x6C];
    char part_84[0x4];
    char pad_88[0x2C];
    char part_B4[0x4];
    char pad_B8[0x20];
    char part_D8[0x4];
    char pad_DC[0x6C];
    int field_148;
    int field_14C;
    void *anim;
} MotionTask;

extern int func_08A22E2C(MotionBrief *info, int a, int b, int kind);
extern float func_08891348(void *anim);
extern void func_08A020C8(void *part);
extern void func_08A00C68(void *part);
extern void func_08A16CC4(void *part, float rate);
extern void func_08A01C74(void *part, int value);
extern void func_08A16C58(void *part);
extern void func_08A00BCC(void *part);

/* Starts the motion and matches its play rate to the animation; falls back when the motion is missing. */
int func_08A189F0(MotionTask *self)
{
    MotionBrief info;
    if (func_08A22E2C(&info, self->field_148, self->field_14C, 2) == 0) {
        if (info.kind == 1) {
            func_08A020C8(self->part_B4);
            func_08A00C68(self->part_D8);
        }
        {
            float length = func_08891348(self->anim);
            if (length != 0.0f) {
                func_08A16CC4(self->part_84, info.duration / length);
            }
        }
    } else {
        func_08A01C74(self->part_14, 9);
        func_08A16C58(self->part_84);
        func_08A00BCC(self->part_D8);
        self->state = 6;
    }
    return 0;
}
