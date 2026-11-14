#include "main/auto_region4.h"

/* Sets the two vectors of the Obj85EA88 at D_089D5EC0 + 0x720 and applies them. */
void func_088E6F0C(void)
{
    Obj85EA88 *object;

    object = &((View9D5EC0 *)D_089D5EC0)->field_720;
    func_0885EAA8(object, D_09AF84C0);
    func_0885EAC4(object, D_09AF8AB0);
    func_0885E860(object);
}
