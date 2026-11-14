#include "menu/auto_region1.h"

extern const short D_08A18370[];
extern const short D_08A18390[];
extern const unsigned short D_08A185C8[];

/* Screen181E0: plays two layout scripts and the text script D_08A185C8. */
void func_08A16384(Screen181E0 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08A18370);
    func_088D3F00(&self->work->layout, self->work->handles, D_08A18390);
    func_088D44EC(self->work->handles, self->work->texts, D_08A185C8);
}
