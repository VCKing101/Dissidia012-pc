/* Release every item of a list: unlink each one and give it back to the stock ring. */
struct Slot0887DA08 {
    void *object;
    Slot0887DA08 *next;
};
struct Item0887DA08 {
    Item0887DA08 *next;
    Item0887DA08 *prev;
};
struct List0887DA08 {
    Item0887DA08 *head;
    Item0887DA08 *tail;
    int count;

    Item0887DA08 *advance(Item0887DA08 *cur) {
        return cur == 0 ? head : cur->next;
    }
    void unlink(Item0887DA08 *item) {
        Item0887DA08 *next = item->next;
        Item0887DA08 *prev = item->prev;
        Item0887DA08 **link;
        if (prev == 0) {
            head = next;
        } else {
            prev->next = next;
        }
        next = item->next;
        prev = item->prev;
        if (next == 0) {
            tail = prev;
        } else {
            link = &next->prev;
            *link = prev;
        }
        item->prev = 0;
        item->next = 0;
        count--;
    }
};
struct Mgr0887DA08 {
    char pad_0000[0xF2B0];
    Slot0887DA08 *take;
    Slot0887DA08 *give;
};
extern "C" void func_0887DA08(Mgr0887DA08 *m, List0887DA08 *list)
{
    Item0887DA08 *it = list->head;
    while (it != 0) {
        Item0887DA08 *nx = list->advance(it);
        list->unlink(it);
        Slot0887DA08 *ret = m->give;
        Slot0887DA08 *slot;
        for (slot = m->take; slot->object != 0; slot = slot->next) {
        }
        ret->object = it;
        m->give = m->give->next;
        it = nx;
    }
}
