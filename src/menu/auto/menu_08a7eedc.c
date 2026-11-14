#include "menu/auto_region3.h"

/* Screen7EEDC: shows (hide == 0) or hides its models and text. */
void func_08A7EEDC(Screen7EEDC *self, int hide, int arg)
{
    if (hide == 0) {
        func_088D9CD8(self->work->field_420);
        func_08A7F398(self, 6, -1);
        func_088D9CD8(self->work->field_418);
        func_08A7F398(self, 4, -1);
        func_088D9CD8(self->work->field_414);
        func_08A7F398(self, 3, -1);
        func_088D9CFC(self->work->field_41C);
        func_088D9CFC(self->work->field_410);
        self->work->field_604 = 0;
        func_08A7F96C(self, 0xC, 1, 0);
        func_088D9CD8(self->work->field_424);
        func_08A80B54(self, 0);
        func_08A7F474(self, 0, arg, -1);
        func_0887B924(self->work->field_594);
        func_08A7F96C(self, 0xE, 0, 0);
    }
    else {
        func_08A7F96C(self, 0xC, 0, 0);
        func_088D9CFC(self->work->field_418);
        func_088D9CFC(self->work->field_410);
        func_088D9CFC(self->work->field_41C);
        func_088D9CFC(self->work->field_414);
        func_088D9CFC(self->work->field_420);
        func_088D9CFC(self->work->field_424);
        func_0887B944(self->work->field_594);
    }
}
