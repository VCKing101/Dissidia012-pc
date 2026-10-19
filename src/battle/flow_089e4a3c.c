#include "battle/runtime_helpers.h"

/* Connected slot/binding/update flow; full class ownership remains unknown. */
extern void func_089E4A94(void *);

int func_089E4A3C(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E4A94(object);
    return 0;
}
