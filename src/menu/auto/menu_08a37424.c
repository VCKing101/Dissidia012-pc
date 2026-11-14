#include "menu/auto_region1.h"

/* Screen38EA8: while field_6C is 0, frees texts 0x15/0x16, resets fields
 * DA8..DB6, redraws and copies field_9C to field_D84 and field_D88. */
void func_08A37424(Screen38EA8 *self)
{
    Work38EA8 *work;

    work = self->work;
    if (work->field_6C == 0) {
        if (work->texts[0x15] != 0) {
            func_0887BA5C(work->texts[0x15]);
            self->work->texts[0x15] = 0;
            work = self->work;
        }
        if (work->texts[0x16] != 0) {
            func_0887BA5C(work->texts[0x16]);
            self->work->texts[0x16] = 0;
            work = self->work;
        }
        work->field_DB5 = 1;
        self->work->field_DB6 = 0;
        self->work->field_DB0 = 0;
        self->work->field_DA8 = 0;
        self->work->field_DAC = 0;
        func_08A382BC(self, 0);
        func_08A35EE8(self, self->work->field_D8C);
        func_08A37E10(self, 0);
        func_08A371E4(self);
        self->work->field_D84 = self->work->field_9C;
        self->work->field_D88 = self->work->field_D84;
    }
}
