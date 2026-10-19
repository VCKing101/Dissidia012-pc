#include "battle/runtime_helpers.h"

/* Observed state/update helpers; original gameplay ownership is unresolved. */
extern void func_089DEDB0(void *);
extern void func_089DEA48(void *);
extern void func_089DEB54(void *);

int func_089DE360(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089DEDB0(object);
    func_089DEA48(object);
    func_089DEB54(object);
    return 0;
}
