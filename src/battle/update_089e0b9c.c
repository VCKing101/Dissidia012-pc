#include "battle/runtime_helpers.h"

/* Observed state/update helpers; original gameplay ownership is unresolved. */
extern int func_0887AFB0(void *);

unsigned char func_089E0B9C(unsigned char *object)
{
    unsigned int result = 0;
    if (object[0] == 1 && func_0887AFB0(object + 8) == 1) result = 1;
    return (unsigned char)result;
}
