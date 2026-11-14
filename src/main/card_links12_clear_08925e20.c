#include "main/agent25_card.h"
void func_08925E20(CardUnit *unit)
{
    int i;
    for (i = 0; i < 120; i++) {
        unit->body->links12[i].id = -1;
        unit->body->links12[i].value = 0;
        unit->body->links12[i].extra = 0;
    }
    unit->body->link12_count = 0;
}
