#include "battle/agent18_types.h"

class FloatStepTask : public BattleTaskBase {
public:
    int field_04;
    char pad_08[0x20 - 0x8];
    char field_20[0x48 - 0x20];
    char field_48[0x78 - 0x48];
    char field_78[0xB0 - 0x78];
    char field_B0[0x4];
};

extern "C" {

extern void func_08A0033C(FloatStepTask *self, float x, float y);
extern void func_08A01948(void *sub);
extern void func_08A169DC(void *sub);
extern void func_089F83AC(void *sub);
extern void func_089F602C(void *sub, float x);

int func_089FFFD8(FloatStepTask *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A0033C(self, x, y);
    func_08A01948(self->field_20);
    func_08A169DC(self->field_48);
    func_089F83AC(self->field_78);
    func_089F602C(self->field_B0, x);
    return 0;
}

}
