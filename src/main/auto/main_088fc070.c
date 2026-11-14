#include "main/auto_region4.h"

void func_088FC070(Obj8FBBE0 *self)
{
    func_088FC87C(self, self->work->texts[0x10]);
    if (self->work->texts[0x10] != 0) {
        func_0887BA5C(self->work->texts[0x10]);
        self->work->texts[0x10] = 0;
    }
}
