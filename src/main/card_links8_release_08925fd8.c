#include "main/agent25_card.h"
extern void func_08925E6C(CardUnit *unit, short id);
void func_08925FD8(CardUnit *unit)
{
    int i;
    for (i = 0; i < unit->body->link8_count; i++) {
        func_08925E6C(unit, unit->body->links8[i].id);
    }
}
