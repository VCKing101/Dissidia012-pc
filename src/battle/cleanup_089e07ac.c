#include "battle/runtime_helpers.h"

/* Observed lifecycle cleanup; original classes and ownership remain unknown. */
extern void *D_089D7830;
extern void func_089E1268(void *);
extern void func_089E0D54(void *);
extern void func_088D0A1C(void *, void *);

void func_089E07AC(unsigned char *object)
{
    int i;
    unsigned char *entry;
    void *resource;
    func_089E0D54(object + 0x88);
    i = 0;
    entry = object + 0x18;
    do {
        func_089E1268(entry);
        ++i;
        entry += 0x1C;
    } while (i < 4);
    resource = ((BattleResourcePrefix *)object)->field_04;
    if (resource) func_088D0A1C(D_089D7830, resource);
    func_089E07A0(object);
}
