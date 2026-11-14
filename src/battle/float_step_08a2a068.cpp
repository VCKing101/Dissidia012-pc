#include "battle/agent18_types.h"

class FloatStepTask : public BattleTaskBase {
public:
    int field_04;
    char pad_08[0xC];
    char field_14[0x4];
    char pad_18[0x24];
    char field_3C[0x4];
};

extern "C" {

extern void func_08A2A2AC(FloatStepTask *self, float x, float y);
extern void func_08A01948(void *sub);
extern void func_08A169DC(void *sub);

int func_08A2A068(FloatStepTask *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A2A2AC(self, x, y);
    func_08A01948(self->field_14);
    func_08A169DC(self->field_3C);
    return 0;
}

}
