/* Runs virtual steps 9 and 8, releases the attached model and clears the state. */
#include "menu/agent16_types.h"

class ModelPart : public A16Part24 {
public:
    int field_28;
    char pad_02C[0x4];
    void *field_30;
    char pad_034[0x434 - 0x34];
    void *field_434;
};

extern "C" void func_08A80EF4(void *model);
extern "C" void func_08A80E34(void *model);
extern "C" void func_08A80D40(void *model, int mode);
extern "C" void func_0885C538(void *heap, void *block, int zero);

extern "C" {
void func_08A6A0F4(ModelPart *self)
{
    self->v9();
    self->v8();
    if (self->field_434 != 0) {
        func_08A80EF4(self->field_434);
        func_08A80E34(self->field_434);
        func_08A80D40(self->field_434, 2);
        func_0885C538(self->field_30, self->field_434, 0);
        self->field_434 = 0;
    }
    self->field_28 = 0;
}
}
