#include "main/auto_region1.h"

/* Runs func_088191A0 on every node of the static Obj9C7530, then clears its
 * field_08. */
void func_0881CC8C(void)
{
    SysNode *node;
    Obj9C7530 *system;

    node = 0;
    while (node = func_0881CB80(node), node != 0) {
        func_088191A0(node);
    }
    system = func_0881BAA8();
    system->field_08 = 0;
}
