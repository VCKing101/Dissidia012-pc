#include "battle/auto_region4.h"

/* Sets flag 1 on every node of the list; returns whether there was any. */
int func_08AE96D4(ListHead *list)
{
    int found;
    Node9520 *node;

    node = 0;
    found = 0;
    while (1) {
        node = node == 0 ? list->first : node->next;
        if (node == 0) {
            break;
        }
        found = 1;
        node->flags = node->flags | 1;
    }
    return found;
}
