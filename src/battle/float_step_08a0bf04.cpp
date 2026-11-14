#include "battle/agent18_types.h"

class FloatStepTask : public BattleTaskBase {
public:
    int field_04;
    char pad_08[0xC];
    char field_14[0x4];
    char pad_18[0x28];
    char field_40[0x4];
};

extern "C" {

extern void func_08A0C2DC(FloatStepTask *self, float x, float y);
extern void func_08A10C88(void *sub);
extern void func_08A08334(void *sub);

int func_08A0BF04(FloatStepTask *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A0C2DC(self, x, y);
    func_08A10C88(self->field_14);
    func_08A08334(self->field_40);
    return 0;
}

}
