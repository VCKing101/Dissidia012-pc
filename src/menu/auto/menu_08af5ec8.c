#include "menu/auto_region3.h"

/* Panel callback: stops handle 0x17 and plays the closing scripts. */
void func_08AF5EC8(void *self, PanelCtx5D20 *ctx)
{
    func_088D9F5C(ctx->handles[0x17], -1, 1);
    func_088D442C(ctx->handles, ctx->texts, ctx->field_54);
    func_088D3C24(ctx->layout, ctx->handles, ctx->field_44, 0x120);
}
