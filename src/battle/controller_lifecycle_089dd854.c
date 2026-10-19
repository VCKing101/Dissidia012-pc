#include "battle/runtime_helpers.h"

/* Battle setup root and its called lifecycle helpers. Subobject names and
 * full layouts are unresolved; byte offsets are the observed ABI. */
extern void func_089ED3A0(void *);

int func_089DD854(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E7F2C(object + 0x1DC);
    func_089E6B4C(object + 0xA8C);
    func_089ED3A0(object + 0x1D04);
    return 0;
}
