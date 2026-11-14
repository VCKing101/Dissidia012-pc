#include "battle/agent18_types.h"

class FloatStepTask : public BattleTaskBase {
public:
    int field_04;
    char pad_08[0x14];
    char field_1C[0x4];
    char pad_20[0x30];
    char field_50[0x4];
};

extern "C" {

extern void func_08A26FA0(FloatStepTask *self, float x, float y);
extern void func_08A08334(void *sub);
extern void func_089F83AC(void *sub);

int func_08A26B8C(FloatStepTask *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A26FA0(self, x, y);
    func_08A08334(self->field_1C);
    func_089F83AC(self->field_50);
    return 0;
}

}
