#include "battle/agent27_types.h"

extern CtxEntry *func_088C91EC(BattleCtx *ctx);

int func_08B2A74C(ResTables *tables)
{
    CtxEntry *entry = func_088C91EC(tables->ctx);

    if (entry == 0) {
        return 0;
    }
    return entry->field_18;
}
