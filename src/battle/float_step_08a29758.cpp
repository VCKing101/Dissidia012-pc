#include "battle/agent18_types.h"

class FloatStepTask : public BattleTaskBase {
public:
    int field_04;
    char pad_08[0x1C - 0x8];
    char field_1C[0x44 - 0x1C];
    char field_44[0x74 - 0x44];
    char field_74[0xAC - 0x74];
    char field_AC[0x4];
};

extern "C" {

extern void func_08A29A64(FloatStepTask *self, float x, float y);
extern void func_08A01948(void *sub);
extern void func_08A169DC(void *sub);
extern void func_089F83AC(void *sub);
extern void func_089F602C(void *sub, float x);

int func_08A29758(FloatStepTask *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    func_08A29A64(self, x, y);
    func_08A01948(self->field_1C);
    func_08A169DC(self->field_44);
    func_089F83AC(self->field_74);
    func_089F602C(self->field_AC, x);
    return 0;
}

}
