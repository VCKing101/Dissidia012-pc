#include "menu/auto_region2.h"

extern const unsigned short D_08A5EFD4[];

/* Panel5EED8: prints field_160 into texts 5 and 6 and hands them out. */
void func_08A5ECCC(Panel5EED8 *self)
{
    void *text;
    unsigned short buffer[0x100];

    func_089291E4(buffer, 0x100, self->field_160, 0x33, 0x19);
    if (self->texts[5] != 0) {
        func_0887BA5C(self->texts[5]);
        self->texts[5] = 0;
    }
    if (self->texts[6] != 0) {
        func_0887BA5C(self->texts[6]);
        self->texts[6] = 0;
    }
    text = func_0887D868(D_089D6998, 0, buffer, -1, 0, 0, 0, 0);
    self->texts[5] = text;
    text = func_0887E534(text);
    self->texts[6] = text;
    func_088D428C(self->handles, self->texts, D_08A5EFD4, 1);
}
