#include "menu/auto_region3.h"

/* ScreenA3F44: reads field_9C4 and field_1F44 back from the screen in field_978. */
void func_08A9A890(ScreenA3F44 *self)
{
    int value;
    struct ScreenA7ED0 *screen;

    screen = self->work->field_978;
    if (screen != 0) {
        func_08AA5544(screen, self->work->field_9C4);
        value = func_08AA55CC(self->work->field_978);
        self->work->field_1F44 = value;
    }
}
