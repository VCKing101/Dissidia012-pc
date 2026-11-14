#include "menu/auto_region3.h"

/* ScreenB01CF8: sets field_2C5C and field_2C60 and runs func_08AF4E64. */
void func_08AF4E98(ScreenB01CF8 *self, unsigned char value)
{
    self->work->field_2C5C = 1;
    self->work->field_2C60 = value;
    func_08AF4E64(self, self->work->field_C0C, self->work->field_2C64);
}
