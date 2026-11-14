#include "battle/agent27_types.h"

extern "C" {

void func_0887BA5C(ListNode *node);
void func_088DA060(void *res);
void func_08B28620(ModelSlot *slot);

struct ModelSlotList {
    static void set(ListNode **link, ListNode *value)
    {
        *link = value;
    }
    static void remove(NodeList *list, ListNode *node)
    {
        if (node == 0) {
            return;
        }
        node->flags &= ~0x8000;
        if (node->prev == 0) {
            list->head = node->next;
        } else {
            set(&node->prev->next, node->next);
        }
        if (node->next == 0) {
            list->tail = node->prev;
        } else {
            set(&node->next->prev, node->prev);
        }
        node->prev = 0;
        node->next = 0;
        list->count--;
    }
};

void func_08B28650(ModelSlot *slot)
{
    if (slot->list != 0 && slot->node != 0) {
        ModelSlotList::remove(slot->list, slot->node);
    }
    if (slot->node != 0) {
        func_0887BA5C(slot->node);
        slot->node = 0;
    }
    if (slot->list != 0) {
        func_088DA060(slot->list);
        slot->list = 0;
    }
    func_08B28620(slot);
}

}
