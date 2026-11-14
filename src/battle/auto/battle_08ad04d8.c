#include "battle/auto_region4.h"

typedef struct OwnerD04D8 {
    char pad_00[0x3C];
    NodeD2ABC *first;
} OwnerD04D8;

/* Calls func_08AD2AE4 on every node of the list at field_3C. */
void func_08AD04D8(OwnerD04D8 *self)
{
    NodeD2ABC *node;

    node = 0;
    while (1) {
        node = node == 0 ? self->first : node->next;
        if (node == 0) {
            break;
        }
        func_08AD2AE4(node);
    }
}
