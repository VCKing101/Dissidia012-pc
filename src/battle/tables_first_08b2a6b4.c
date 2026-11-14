#include "battle/agent27_types.h"

extern CtxEntry *func_088C8488(BattleCtx *ctx);

int func_08B2A6B4(ResTables *tables)
{
    CtxEntry *entry = func_088C8488(tables->ctx);

    if (entry == 0) {
        return 0;
    }
    return entry->field_18;
}
