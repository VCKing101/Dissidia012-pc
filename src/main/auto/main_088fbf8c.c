#include "main/auto_region4.h"

/* Obj8FBBE0: hands texts 0xE..0x12 to func_088FC87C, then releases them. */
void func_088FBF8C(Obj8FBBE0 *self)
{
    int i;

    for (i = 0xe; i < 0x13; i++) {
        func_088FC87C(self, self->work->texts[i]);
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
}
