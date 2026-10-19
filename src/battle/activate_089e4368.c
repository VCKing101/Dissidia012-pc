#include "battle/runtime_helpers.h"

/* Connected activation/state helpers; complete class ownership is unknown. */
extern void func_089E439C(void *);

int func_089E4368(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E439C(object);
    return 0;
}
