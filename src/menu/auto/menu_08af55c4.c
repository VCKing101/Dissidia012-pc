#include "menu/auto_region3.h"

/* Panel callback: plays the context's script. */
void func_08AF55C4(void *self, PanelCtx55C4 *ctx)
{
    func_088D3F00(ctx->layout, ctx->handles, ctx->script);
}
