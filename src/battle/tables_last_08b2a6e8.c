#include "battle/agent27_types.h"

extern float D_08B2BB00;
extern CtxEntry *func_088C84E8(BattleCtx *ctx);

int func_08B2A6E8(ResTables *tables)
{
    CtxEntry *entry = func_088C84E8(tables->ctx);

    if (entry == 0) {
        return (int)D_08B2BB00;
    }
    return entry->field_18;
}
