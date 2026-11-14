#include "battle/agent23_types.h"

typedef struct MotionTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x8];
    char player[0x28];
    char rate[0x30];
    int set;
    int index;
    void *anim;
} MotionTask;

extern int func_08A22E2C(MotionBrief *info, int set, int index, int kind);
extern float func_08891348(void *anim);
extern int func_08A01BC8(void *player, int motion, int frame);
extern void func_08A16CC4(void *rate, float value);
extern void func_08A01C74(void *player, int value);
extern void func_08A16C58(void *rate);

/* Starts motion 4 and matches its play rate to the animation; falls back to state 3. */
int func_08A0D508(MotionTask *self)
{
    MotionBrief info;
    if (func_08A22E2C(&info, self->set, self->index, 4) == 0) {
        if (info.kind == 1) {
            func_08A01BC8(self->player, 2, 1);
        }
        {
            float length = func_08891348(self->anim);
            if (length != 0.0f) {
                func_08A16CC4(self->rate, info.duration / length);
            }
        }
    } else {
        func_08A01C74(self->player, 3);
        func_08A16C58(self->rate);
        self->state = 3;
    }
    return 0;
}
