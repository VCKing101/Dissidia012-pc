#include "menu/auto_region1.h"

extern const unsigned short D_089E6DEE[];
extern const unsigned short D_089E6DFE[];
extern const short D_089E6722[];
extern const short D_089E6942[];
extern const short D_089E696A[];

/* ScreenE75C4: plays two text scripts, places handle 0x31 at
 * (field_A8C, field_A90), then plays three layout scripts. */
void func_089E1E5C(ScreenE75C4 *self)
{
    WorkE75C4 *work;

    func_088D44EC(self->work->handles, self->work->texts, D_089E6DEE);
    func_088D44EC(self->work->handles, self->work->texts, D_089E6DFE);
    work = self->work;
    func_088D3AE8(work->handles, work->field_A8C, work->field_A90, 0x31);
    func_088D3F00(&self->work->layout, self->work->handles, D_089E6722);
    func_088D3F00(&self->work->layout, self->work->handles, D_089E6942);
    func_088D3F00(&self->work->layout, self->work->handles, D_089E696A);
}
