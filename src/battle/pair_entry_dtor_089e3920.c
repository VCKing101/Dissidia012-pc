#include "battle/agent1_types.h"

/* Destructor of one pair entry. */
extern unsigned char D_089E3EA8[];
extern void func_089E39CC(BattlePairEntry *);
extern void func_089E3F40(void *, int);
extern void func_089E44F8(void *, int);
extern void func_08992F04(void *);

void func_089E3920(BattlePairEntry *entry, int flags)
{
    if (entry) {
        entry->vtable = D_089E3EA8;
        func_089E39CC(entry);
        func_089E3F40(&entry->field_70, 2);
        func_089E44F8(&entry->field_3C, 2);
        func_089E44F8(&entry->field_08, 2);
        if (flags & 1) func_08992F04(entry);
    }
}
