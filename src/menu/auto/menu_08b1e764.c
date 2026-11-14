#include "menu/auto_region4.h"

extern const short D_08B228D0[];
extern const short D_08B228E8[];
extern const short D_08B22A10[];
extern const short D_08B22E70[];
extern const short D_08B22E7A[];

/* ScreenB22E98: fills the panel context with its layout and scripts. */
void func_08B1E764(ScreenB22E98 *self)
{
    self->work->ctx.layout = &self->work->layout;
    self->work->ctx.handles = self->work->handles;
    self->work->ctx.texts = self->work->texts;
    self->work->ctx.field_44 = D_08B228D0;
    self->work->ctx.field_48 = D_08B228E8;
    self->work->ctx.field_5C = D_08B22E70;
    self->work->ctx.field_60 = D_08B22E7A;
    self->work->ctx.field_50 = D_08B22A10;
    self->work->ctx.field_58 = 2;
}
