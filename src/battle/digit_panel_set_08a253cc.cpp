#include "battle/agent31_types.h"

extern "C" void func_088DA060(void *sprite);
extern "C" void *func_088DAA40(void *sheet, int frame, int flags);
extern "C" void func_08A24ED4(A31DigitPanel *self);

extern "C" int func_08A253CC(A31DigitPanel *self, int value)
{
    if (!self->open) {
        return -1;
    }
    for (int i = 0; i < 4; i++) {
        if (self->digits[i] != 0) {
            func_088DA060(self->digits[i]);
            self->digits[i] = 0;
        }
    }
    if (value < 0) {
        value = 0;
    } else if (value >= 10000) {
        value = 9999;
    }
    for (int i = 0; i < 4; i++) {
        if (value == 0 && i != 0) {
            continue;
        }
        value /= 10;
        self->digits[i] = func_088DAA40(self->sheet, 0, 0);
    }
    func_08A24ED4(self);
    return 0;
}
