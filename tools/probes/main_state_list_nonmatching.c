/* Uncredited probe only: the pinned SNC profile produces different branch
 * forms and register allocation. These functions are absent from matches. */
#include "main/state_list.h"

MainStateListNode *func_088651AC(MainStateListNode *node) {
    MainStateListNode *result;
    if (!node) result = D_089D5EB0->head;
    else result = node->next;
    return result;
}

void func_088651CC(MainStateListNode *node) {
    MainStateListNode *head = D_089D5EB0->head;
    if (!head) {
        D_089D5EB0->tail = node;
        D_089D5EB0->head = node;
        node->next = 0;
    } else {
        node->next = head;
        D_089D5EB0->head = node;
    }
}

void func_08865204(MainStateListNode *node) {
    MainStateListPrefix *list = D_089D5EB0;
    MainStateListNode *next;
    MainStateListNode *previous = list->head;
    if (previous && list->tail) {
        next = node->next;
        if (node == previous) {
            list->head = next;
            previous = 0;
        } else {
            if (previous->next != node) {
                do { previous = previous->next; } while (previous->next != node);
            }
            previous->next = next;
        }
        if (node == list->tail) list->tail = previous;
        node->next = 0;
    }
}
