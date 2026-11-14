#include "main/auto_region2.h"

/* Clears both flags, the word at field_5C and the member at field_74. */
void func_08886934(Obj886934 *self)
{
    self->field_58 = 0;
    self->field_59 = 0;
    func_0885EB60(&self->field_5C);
    func_0888FA08(self->field_74);
}
