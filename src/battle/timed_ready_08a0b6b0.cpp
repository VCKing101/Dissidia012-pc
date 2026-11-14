#include "battle/agent11_types.h"

class WindowTask : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x4];
    float elapsed;
};

extern "C" {

extern float D_08A0BBA0;
extern float D_08A0BBA4;

/* 1 when the task is in state 1 or 6 and has run past its window. */
unsigned char func_08A0B6B0(WindowTask *self)
{
    if (!self->isActive()) {
        return 0;
    }
    int result = 0;
    if ((self->state == 1 || self->state == 6) && self->elapsed >= D_08A0BBA4 - D_08A0BBA0) {
        result = 1;
    }
    return result;
}

}
