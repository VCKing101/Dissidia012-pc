#include "battle/agent27_types.h"

extern void func_08B29FF0(ResTables *tables);

int func_08B2A0B8(ResTables *tables, BattleCtx *ctx, int value)
{
    func_08B29FF0(tables);
    if (ctx == 0 || value == 0) {
        return -1;
    }
    tables->ctx = ctx;
    tables->field_1B4 = value;
    tables->active = 1;
    return 0;
}
