#include "battle/runtime_helpers.h"

/* Observed lifecycle cleanup; original classes and ownership remain unknown. */
extern void *D_089D7830;
extern void func_089ECFD0(void *);
extern void func_088D0A1C(void *, void *);

void func_089ED01C(unsigned char *object)
{
    void *resource = ((BattleResourcePrefix *)object)->field_04;
    if (resource) func_088D0A1C(D_089D7830, resource);
    func_089ECFD0(object);
}
