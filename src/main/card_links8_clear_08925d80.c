#include "main/agent25_card.h"
void func_08925D80(CardUnit *unit)
{
    int i;
    for (i = 0; i < 120; i++) {
        unit->body->links8[i].id = -1;
        unit->body->links8[i].value = 0;
    }
    unit->body->link8_count = 0;
}
