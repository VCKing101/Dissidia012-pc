#include "menu/auto_region1.h"

extern const short D_08A119BE[];

/* Screen11510: plays D_08A119BE and redraws its parts. */
void func_08A0D948(Screen11510 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08A119BE);
    func_08A0D4A4(self);
    func_088D4B74(&self->work->field_10F0, 0);
    func_08A0CD80(self);
    func_08A0C990(self);
    func_08A0D0F0(self);
    func_08A0CBC0(self);
}
