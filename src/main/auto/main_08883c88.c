#include "main/auto_region2.h"

/* The node named name, or 0. */
Node883C88 *func_08883C88(List883C88 *self, const char *name)
{
    Node883C88 *node;
    int diff;

    node = 0;
    while (1) {
        node = (node == 0) ? self->first : node->next;
        if (node == 0) break;
        diff = func_0893585C(name, node->name);
        if (diff == 0) {
            return node;
        }
    }
    return 0;
}
