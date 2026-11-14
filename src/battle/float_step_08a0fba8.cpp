#include "battle/agent18_types.h"

class FloatStepTask : public BattleTaskBase {
public:
    int field_04;
    char pad_08[0x8];
    char field_10[0x4];
};

extern "C" {

extern void func_08A0FEEC(FloatStepTask *self, float x, float y);
extern void func_08A0EFD0(void *sub, float x);

int func_08A0FBA8(FloatStepTask *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A0FEEC(self, x, y);
    func_08A0EFD0(self->field_10, x);
    return 0;
}

}
