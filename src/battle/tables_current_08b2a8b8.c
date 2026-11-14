#include "battle/agent27_types.h"

extern CtxEntry *func_088C90C0(BattleCtx *ctx);

int func_08B2A8B8(ResTables *tables)
{
    CtxEntry *entry = func_088C90C0(tables->ctx);

    if (entry == 0) {
        return 0;
    }
    return entry->field_18;
}
