#include "battle/runtime_helpers.h"

/* Connected activation/state helpers; complete class ownership is unknown. */
extern int func_0887AFB0(void *);

int func_089EA0AC(unsigned char *object, int value)
{
    if (!func_089EA11C(object)) return -1;
    {
        int i;
        unsigned char *entry;
        i = 0;
        entry = object + 0x1C;
        do {
            extern void func_089EA8F8(void *, int);
            func_089EA8F8(entry, value);
            ++i;
            entry += 0x84C;
        } while (i < 2);
    }
    return 0;
}

unsigned char func_089EA11C(unsigned char *object)
{
    unsigned int result = 0;
    if (object[0] == 1 && func_0887AFB0(object + 8) == 1) result = 1;
    return (unsigned char)result;
}
