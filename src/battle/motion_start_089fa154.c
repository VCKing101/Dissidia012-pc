#include "battle/agent23_types.h"

typedef struct MotionTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x8];
    char part_14[0x4];
    char pad_18[0x24];
    char part_3C[0x4];
    char pad_40[0x2C];
    char part_6C[0x4];
    char pad_70[0x20];
    char part_90[0x4];
    char pad_94[0x6C];
    int field_100;
    int field_104;
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
int func_089FA154(MotionTask *self)
{
    MotionBrief info;
    if (func_08A22E2C(&info, self->field_100, self->field_104, 2) == 0) {
        if (info.kind == 1) {
            func_08A020C8(self->part_6C);
            func_08A00C68(self->part_90);
        }
        {
            float length = func_08891348(self->anim);
            if (length != 0.0f) {
                func_08A16CC4(self->part_3C, info.duration / length);
            }
        }
    } else {
        func_08A01C74(self->part_14, 7);
        func_08A16C58(self->part_3C);
        func_08A00BCC(self->part_90);
        self->state = 3;
    }
    return 0;
}
