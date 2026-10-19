#include "battle/runtime_helpers.h"

/* Battle setup root and its called lifecycle helpers. Subobject names and
 * full layouts are unresolved; byte offsets are the observed ABI. */
extern int func_089E8EF0(void *);
extern int func_089E8F50(void *);

int func_089DDAF0(unsigned char *object)
{
    if (!object[0]) return -1;
    return func_089E8EF0(object + 0x10);
}

int func_089DDB20(unsigned char *object)
{
    if (!object[0]) return -1;
    return func_089E8F50(object + 0x10);
}
