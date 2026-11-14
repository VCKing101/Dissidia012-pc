#include "battle/agent18_types.h"

class FloatStepTask : public BattleTaskBase {
public:
    int field_04;
    char pad_08[0xC];
    char field_14[0x4];
};

extern "C" {

extern void func_089F1940(FloatStepTask *self, float x, float y);
extern void func_089F1D2C(void *sub);

int func_089F16AC(FloatStepTask *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_089F1940(self, x, y);
    func_089F1D2C(self->field_14);
    return 0;
}

}
