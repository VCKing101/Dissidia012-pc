#include "menu/auto_region3.h"

/* Panel callback: plays a motion on handle 0x17. */
void func_08AF5D20(void *self, PanelCtx5D20 *ctx, int motion)
{
    func_088D9F5C(ctx->handles[0x17], motion, 1);
}
