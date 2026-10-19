#include "battle/runtime_helpers.h"

/* Battle setup root and its called lifecycle helpers. Subobject names and
 * full layouts are unresolved; byte offsets are the observed ABI. */
extern void func_089DD5D4(void *);

void *func_089DD4D0(unsigned char *object)
{
    func_089E8E70(object + 0x10);
    func_089E3208(object + 0x18);
    func_089E79A8(object + 0x1DC);
    func_089E7130(object + 0xA44);
    func_089E68E0(object + 0xA8C);
    func_089E06A8(object + 0x1C6C);
    func_089ECF40(object + 0x1D04);
    func_089DD5D4(object);
    return object;
}
