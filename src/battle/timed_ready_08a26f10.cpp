#include "battle/agent11_types.h"

class TimedTask : public BattleTaskBase {
public:
    int field_04;
    int state;
    char pad_0C[0x4];
    float elapsed;
    unsigned char done;
};

extern "C" {

extern float D_08A272B8;

/* 1 once the task has waited out its limit in state 1 with its done flag set. */
unsigned char func_08A26F10(TimedTask *self)
{
    if (!self->isActive()) {
        return 0;
    }
    int result = 0;
    if (self->state == 1 && self->elapsed >= D_08A272B8 && self->done == 1) {
        result = 1;
    }
    return result;
}

}
