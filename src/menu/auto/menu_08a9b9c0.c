#include "menu/auto_region3.h"

/* ScreenA3F44: reads field_9C4, field_82F4 and field_7C1C back from the screen in field_8044. */
void func_08A9B9C0(ScreenA3F44 *self)
{
    int value;
    struct ScreenAB7CFC *screen;

    screen = self->work->field_8044;
    if (screen != 0) {
        func_08AAB5B4(screen, self->work->field_9C4);
        value = func_08AAB624(self->work->field_8044);
        self->work->field_82F4 = value;
        value = func_08AAB630(self->work->field_8044);
        self->work->field_7C1C = value;
    }
}
