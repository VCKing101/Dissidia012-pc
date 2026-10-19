#include "battle/runtime_helpers.h"

/* Observed state/update helpers; original gameplay ownership is unresolved. */
extern void func_089E3CDC(void *);
extern void func_089E4674(void *);
extern void func_089E4060(void *);

int func_089E3B0C(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E3CDC(object);
    func_089E4674(object + 8);
    func_089E4674(object + 0x3C);
    func_089E4060(object + 0x70);
    return 0;
}
