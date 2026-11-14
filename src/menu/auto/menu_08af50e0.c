#include "menu/auto_region3.h"

extern const unsigned short D_08B00C58[];

/* Panel callback: makes the texts of script D_08B00C58. */
void func_08AF50E0(void *self, PanelCtx50E0 *ctx)
{
    func_088D3F7C(ctx->messages, ctx->texts, D_08B00C58, 0);
}
