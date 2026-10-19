#include "battle/runtime_helpers.h"

/* Connected activation/state helpers; complete class ownership is unknown. */
extern void func_089E4EE0(void *);
extern void func_089EE014(void *);

int func_089E5988(unsigned char *object)
{
    func_089E63D4(object + 0x10);
    func_089E5B60((BattleControlState *)(object + 0x40));
    func_089E4EE0(object + 0x58);
    func_089EE014(object + 0x84);
    return 0;
}
