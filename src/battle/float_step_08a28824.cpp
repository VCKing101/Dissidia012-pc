#include "battle/agent18_types.h"

class FloatStepTask : public BattleTaskBase {
public:
    int field_04;
    char pad_08[0xC];
    char field_14[0x4];
};

extern "C" {

extern void func_08A28A40(FloatStepTask *self, float x, float y);
extern void func_08A08334(void *sub);

int func_08A28824(FloatStepTask *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A28A40(self, x, y);
    func_08A08334(self->field_14);
    return 0;
}

}
