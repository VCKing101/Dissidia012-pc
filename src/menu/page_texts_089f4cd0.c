#include "menu/agent29_types.h"

/* Screen0B6A0: plays the page script, hides handle 0x9F unless the save
   has the entry +0x272E, makes texts 0x16/0x17 and places the texts. */
extern const unsigned short D_08A09E0A[];
extern const unsigned short D_08A09E3A[];
extern int func_088F3D48(void *table, unsigned short id);

void func_089F4CD0(Screen0B6A0 *self)
{
    func_088D3CF0(&self->work->layout, self->work->handles, D_08A09E0A);
    if (func_088F3D48((char *)D_089D712C + 0x1E208, WORK_U16(self->work, 0x272E)) == 0) {
        func_088D9CFC(self->work->handles[0x9F]);
    }
    self->work->texts[0x16] = func_089EF1E8(self, 0, 0x7B, 0, 0);
    self->work->texts[0x17] = func_089EF1E8(self, 0, 0x7C, 0, 0);
    func_088D428C(self->work->handles, self->work->texts, D_08A09E3A, 1);
    func_089F3F68(self, 0xA5);
    func_089F3F68(self, 0xAA);
}
