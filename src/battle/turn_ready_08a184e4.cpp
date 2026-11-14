#include "battle/agent11_types.h"

class TurnTask : public BattleTask {
public:
    char pad_14[0x134];
    int field_148;
    int field_14C;
    char pad_150[0x4];
    int field_154;
};

extern "C" {
extern int func_089F5C78(int target, int parity);
}

struct TurnOps {
    static unsigned char ready(TurnTask *self)
    {
        int parity = (self->field_148 + self->field_14C) % 2;
        int result = 0;
        if ((self->state == 4 && func_089F5C78(self->field_154, parity) == 1) || self->state == 10) {
            result = 1;
        }
        return result;
    }
};

extern "C" {

unsigned char func_08A184E4(TurnTask *self)
{
    if (!self->isActive()) {
        return 0;
    }
    return TurnOps::ready(self);
}

}
