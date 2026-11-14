#include "main/agent25_card.h"
int func_08926048(CardUnit *unit, short id)
{
    int i;
    int result = 0;
    for (i = 0; i < unit->body->link12_count; i++) {
        if (unit->body->links12[i].id == id) {
            result = unit->body->links12[i].value;
            break;
        }
    }
    return result;
}
