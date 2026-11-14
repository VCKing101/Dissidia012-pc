#include "battle/agent11_types.h"

typedef struct MotionTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0xC0];
    char field_CC[0x4];
    char pad_D0[0x24];
    char field_F4[0x4];
    char pad_F8[0x2C];
    char field_124[0x4];
    char pad_128[0x30];
    char field_158[0x4];
    char pad_15C[0x108];
    int field_264;
    int field_268;
    void *anim;
} MotionTask;

extern int func_08A22E2C(MotionInfo *info, int a, int b, int kind);
extern int func_08891710(void *anim, char *name, int kind);
extern float func_08891348(void *anim);
extern void func_089FE1BC(void *sub, int index);
extern void func_08A16CC4(void *sub, float rate);
extern void func_08A01C74(void *sub, int value);
extern void func_08A16C58(void *sub);
extern void func_089FC308(void *sub);

int func_08A1DF40(MotionTask *self)
{
    MotionInfo info;
    if (func_08A22E2C(&info, self->field_264, self->field_268, 8) == 0) {
        if (info.kind == 1) {
            if (func_08891710(self->anim, info.name, 8) != 0) {
                return -1;
            }
            {
                int index = info.index;
                func_089FE1BC(self->field_124, index);
            }
        }
        {
            float length = func_08891348(self->anim);
            if (length != 0.0f) {
                func_08A16CC4(self->field_F4, info.duration / length);
            }
        }
    } else {
        func_08A01C74(self->field_CC, 8);
        func_08A16C58(self->field_F4);
        func_089FC308(self->field_158);
        self->state = 5;
    }
    return 0;
}
