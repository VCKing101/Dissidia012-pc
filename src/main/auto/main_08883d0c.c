#include "main/auto_region2.h"

/* The node named name, or 0. */
Node883D0C *func_08883D0C(List883D0C *self, const char *name)
{
    Node883D0C *node;
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
