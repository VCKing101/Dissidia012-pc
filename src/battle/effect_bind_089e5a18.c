#include "battle/agent1_types.h"

/* Constructor, destructor, release and setup of the effect binding. */
extern unsigned char D_089E6068[];
extern void func_089E5AA8(BattleEffectBind *);
extern void func_08992F04(void *);
void func_089E5AD0(BattleEffectBind *bind);

BattleEffectBind *func_089E5A18(BattleEffectBind *bind)
{
    bind->vtable = D_089E6068;
    func_089E5AA8(bind);
    return bind;
}

void func_089E5A50(BattleEffectBind *bind, int flags)
{
    if (bind) {
        bind->vtable = D_089E6068;
        func_089E5AD0(bind);
        if (flags & 1) func_08992F04(bind);
    }
}
