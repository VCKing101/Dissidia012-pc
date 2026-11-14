#include "menu/auto_region3.h"

extern const unsigned short D_08B00E6A[];

/* Panel callback: stops handle 0x17 and plays the closing scripts (texts D_08B00E6A). */
void func_08AF63CC(void *self, PanelCtx5D20 *ctx)
{
    func_088D9F5C(ctx->handles[0x17], -1, 1);
    func_088D442C(ctx->handles, ctx->texts, D_08B00E6A);
    func_088D3C24(ctx->layout, ctx->handles, ctx->field_44, 0x120);
}
