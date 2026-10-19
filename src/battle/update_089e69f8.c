#include "battle/runtime_helpers.h"

/* Observed state/update helpers; original gameplay ownership is unresolved. */
extern void func_089E9F74(void *, void *, void *);
extern void func_089E6CE4(void *);

int func_089E69F8(unsigned char *object, void *field_a, void *field_b)
{
    func_089E69C0(object);
    if (!field_a || !field_b) return -1;
    func_089E1EC0(object + 8, field_b);
    func_089E9F74(object + 0x124, field_a, field_b);
    object[0] = 1;
    return 0;
}

int func_089E6A74(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E6CE4(object);
    func_089E1FD0(object + 8);
    func_089EA064(object + 0x124);
    return 0;
}
