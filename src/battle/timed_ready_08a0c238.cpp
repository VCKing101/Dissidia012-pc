#include "battle/agent11_types.h"

extern "C" {
extern float D_08A0C5F0;
extern float D_08A0C5F4;
}

class WindowTask : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x4];
    float elapsed;

    static unsigned char ready(WindowTask *self)
    {
        int result = 0;
        if (((self->state == 1 || self->state == 5) && self->elapsed >= D_08A0C5F4 - D_08A0C5F0) || self->state == 2) {
            result = 1;
        }
        return result;
    }
};

extern "C" {

/* 1 in state 2, or in state 1 or 5 once the task has run past its window. */
unsigned char func_08A0C238(WindowTask *self)
{
    if (!self->isActive()) {
        return 0;
    }
    return WindowTask::ready(self);
}

}
