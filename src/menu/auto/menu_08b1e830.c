#include "menu/auto_region4.h"

extern const short D_08B228F8[];
extern const unsigned short D_08B2293A[];
extern const short D_08B22992[];
extern const unsigned short D_08B229C8[];

/* ScreenB22E98: opens the panel picked by field_80F8. */
void func_08B1E830(ScreenB22E98 *self)
{
    WorkB22E98 *work;

    work = self->work;
    work->ctx.field_00 = work->field_8080[work->field_80F8];
    work = self->work;
    if (work->field_80F8 == 0) {
        work->ctx.field_4C = D_08B228F8;
        self->work->ctx.field_54 = D_08B2293A;
        self->work->ctx.field_04 = 0;
        self->work->ctx.field_08 = 4;
        work = self->work;
    }
    else {
        work->ctx.field_4C = D_08B22992;
        self->work->ctx.field_54 = D_08B229C8;
        self->work->ctx.field_04 = 0;
        self->work->ctx.field_08 = 3;
        work = self->work;
    }
    func_08AF5C34(D_08B01CF8, &work->ctx, 3);
}
