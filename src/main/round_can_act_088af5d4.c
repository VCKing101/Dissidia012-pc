/* 1 when the value is negative and the round is not in modes 4 to 8. */
#include "main/agent17_types.h"

int func_088AF5D4(Round *round, int value)
{
    RoundCtx *ctx = round->ctx;
    if (value < 0) {
        int mode = ctx->mode;
        if (mode != 5 && mode != 4 && mode != 6 && mode != 7 && mode != 8) {
            return 1;
        }
    }
    return 0;
}
