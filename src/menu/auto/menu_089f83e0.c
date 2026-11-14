#include "menu/auto_region1.h"

extern const short D_08A096A2[];
extern const short D_08A0A152[];
extern const short D_08A096C2[];
extern const short D_08A097BA[];
extern const short D_08A09A4A[];
extern const short D_08A09AFA[];
extern const short D_08A09B1A[];

/* Screen0B6A0: plays its layout scripts and redraws every part. */
void func_089F83E0(Screen0B6A0 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08A096A2);
    func_089FC9FC(self);
    func_088D3F00(&self->work->layout, self->work->handles, D_08A0A152);
    func_089FF848(self);
    func_089F2D48(self);
    func_089F3B70(self);
    func_088D3F00(&self->work->layout, self->work->handles, D_08A096C2);
    func_088D3F00(&self->work->layout, self->work->handles, D_08A097BA);
    func_088D3F00(&self->work->layout, self->work->handles, D_08A09A4A);
    func_089F5964(self);
    func_088D3F00(&self->work->layout, self->work->handles, D_08A09AFA);
    func_089F3498(self, 0);
    func_088D3F00(&self->work->layout, self->work->handles, D_08A09B1A);
    func_089F3DF4(self);
    func_089F63C8(self);
    func_089F6AF4(self);
    func_089F6D58(self);
    func_089F6E58(self);
    func_089F3790(self);
    func_089F70F0(self);
    func_089F74E4(self);
    func_089F777C(self);
    func_089F7B8C(self);
    func_089F7D8C(self);
    func_08A00D98(self);
    func_089F30E8(self);
}
