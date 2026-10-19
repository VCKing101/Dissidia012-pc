#include "battle/runtime_helpers.h"

/* Battle setup root and its called lifecycle helpers. Subobject names and
 * full layouts are unresolved; byte offsets are the observed ABI. */
extern void func_089E3414(void *);
extern void func_089E7D60(void *);
extern void func_089ED0B8(void *);

int func_089DD7A8(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E3414(object + 0x18);
    func_089E7D60(object + 0x1DC);
    func_089E7318(object + 0xA44);
    func_089E6A74(object + 0xA8C);
    func_089E0904(object + 0x1C6C);
    func_089ED0B8(object + 0x1D04);
    return 0;
}
