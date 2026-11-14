#include "battle/auto_region5.h"

/* Part452B4: runs func_08B44CFC on every node of its list, then sets
 * field_04. */
void func_08B452B4(Part452B4 *self)
{
    Node452B4 *node;

    node = 0;
    while (1) {
        node = node == 0 ? self->first : node->next;
        if (node == 0) {
            break;
        }
        func_08B44CFC(node);
    }
    self->field_04 = 1;
}
