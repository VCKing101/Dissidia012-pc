#include "battle/runtime_helpers.h"

/* Battle setup root and its called lifecycle helpers. Subobject names and
 * full layouts are unresolved; byte offsets are the observed ABI. */
extern void func_089E0C6C(void *);

void func_089DDE4C(unsigned char *object)
{
    func_089E0C6C(object + 0x1C6C);
    func_089E6CC8(object + 0xA8C);
}
