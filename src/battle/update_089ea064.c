#include "battle/runtime_helpers.h"

/* Observed state/update helpers; original gameplay ownership is unresolved. */
extern void func_089EA448(void *);
extern void func_089EA450(void *);

int func_089EA064(unsigned char *object)
{
    if (!func_089EA11C(object)) return -1;
    func_089EA448(object);
    func_089EA450(object);
    return 0;
}
