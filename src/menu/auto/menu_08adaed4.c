#include "menu/auto_region3.h"

extern const unsigned short D_08ADF8E4[];

/* Screen46AC: plays D_08ADF8E4 (looped form). */
void func_08ADAED4(Screen46AC *self)
{
    func_088D3CF0(&self->work->layout, self->work->handles, D_08ADF8E4);
}
