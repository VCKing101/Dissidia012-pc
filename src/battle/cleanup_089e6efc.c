#include "battle/runtime_helpers.h"

/* Observed lifecycle cleanup; original classes and ownership remain unknown. */
extern void func_088DA060(void *);

void func_089E6EFC(unsigned char *object)
{
    void *resource = ((BattleResourcePrefix *)object)->field_04;
    if (resource) {
        func_088DA060(resource);
        ((BattleResourcePrefix *)object)->field_04 = 0;
    }
    func_089E6EF0(object);
}
