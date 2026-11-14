#include "menu/auto_region3.h"

/* Screen850: sets entry 0xF (from field_5C0 or field_5C8) and entry 0x31 (from field_5C8). */
void func_08AE0638(Screen850 *self)
{
    int value;

    value = self->work->field_5C0;
    if (value == 0) {
        func_08AE09D8(self, 0xF, self->work->field_5C8 + 0x3E);
        value = self->work->field_5C8;
    }
    else {
        func_08AE09D8(self, 0xF, value + 0x15);
        value = self->work->field_5C8;
    }
    func_08AE09D8(self, 0x31, value + 8);
}
