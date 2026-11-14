/* Ends the round in mode 6 (kind 8) or 7 and resets the round state. */
#include "main/agent17_types.h"

void func_088AF134(Round *round, int mode);

void func_088AF36C(Round *round, int kind)
{
    RoundCtx *ctx = round->ctx;
    ctx->mode = kind == 8 ? 6 : 7;
    round->ctx->field_DD4 = 0;
    round->ctx->stage = 1;
    round->ctx->field_2B80 = 0;
    round->ctx->field_D65 = 0;
    round->flags.word &= ~1;
    func_088AF134(round, 0);
}
