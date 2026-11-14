#include "battle/agent28_types.h"

/* Starts waiting records while the running count is under the limit and
 * removes finished records from the active list. */
struct CardList {
    static CardRecord *next(CardItemState *item, CardRecord *record)
    {
        return !record ? item->head : record->next;
    }
    static void set(CardRecord **slot, CardRecord *value) { *slot = value; }
    static void unlink(CardItemState *item, CardRecord *record)
    {
        if (record->prev == 0) {
            item->head = record->next;
        } else {
            record->prev->next = record->next;
        }
        if (record->next == 0) {
            item->tail = record->prev;
        } else {
            set(&record->next->prev, record->prev);
        }
        record->prev = 0;
        record->next = 0;
        item->count--;
    }
};

extern "C" {

extern int func_089EB6AC(CardItemState *item);
extern int func_089EB6D8(CardItemState *item);
extern int func_089EC260(CardRecord *record);
extern void func_089EC114(CardRecord *record);
extern int func_089EC288(CardRecord *record);
extern void func_089EADA4(CardItemState *item, CardRecord *record);

int func_089EAFB0(CardItemState *item)
{
    int limit = func_089EB6AC(item);
    int running = func_089EB6D8(item);
    CardRecord *record = item->head;
    while (record != 0) {
        CardRecord *next = CardList::next(item, record);
        if (func_089EC260(record) == 1 && running < limit) {
            func_089EC114(record);
            running++;
        }
        if (func_089EC288(record) == 1) {
            func_089EADA4(item, record);
            CardList::unlink(item, record);
        }
        record = next;
    }
    return 0;
}

}
