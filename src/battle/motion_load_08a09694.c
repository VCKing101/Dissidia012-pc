#include "battle/agent11_types.h"

typedef struct MotionTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x8];
    char field_14[0x4];
    char pad_18[0x24];
    char field_3C[0x4];
    char pad_40[0x2C];
    char field_6C[0x4];
    char pad_70[0x6C];
    int field_DC;
    int field_E0;
    void *anim;
} MotionTask;

extern int func_08A22E2C(MotionInfo *info, int a, int b, int kind);
extern int func_08891710(void *anim, char *name, int kind);
extern float func_08891348(void *anim);
extern void func_08A08C54(void *sub, int index);
extern void func_08A16CC4(void *sub, float rate);
extern void func_08A01C74(void *sub, int value);
extern void func_08A16C58(void *sub);
extern void func_08A08E0C(void *sub);

int func_08A09694(MotionTask *self)
{
    MotionInfo info;
    if (func_08A22E2C(&info, self->field_DC, self->field_E0, 28) == 0) {
        if (info.kind == 1) {
            if (func_08891710(self->anim, info.name, 28) != 0) {
                return -1;
            }
            {
                int index = info.index;
                func_08A08C54(self->field_6C, index);
            }
        }
        {
            float length = func_08891348(self->anim);
            if (length != 0.0f) {
                func_08A16CC4(self->field_3C, info.duration / length);
            }
        }
    } else {
        func_08A01C74(self->field_14, 5);
        func_08A16C58(self->field_3C);
        func_08A08E0C(self->field_6C);
        self->state = 4;
    }
    return 0;
}
