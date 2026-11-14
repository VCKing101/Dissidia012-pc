#include "battle/agent28_types.h"

/* Tells the item's indicator whether any active record is still playing. */
struct CardList {
    static CardRecord *next(CardItemState *item, CardRecord *record)
    {
        return !record ? item->head : record->next;
    }
};

extern "C" {

extern int func_089EC1CC(CardRecord *record);
extern int func_089EC210(CardRecord *record);
extern int func_089EB9CC(void *indicator, unsigned char busy);

int func_089EB54C(CardItemState *item)
{
    unsigned char busy = 0;
    CardRecord *record = 0;
    while ((record = CardList::next(item, record)) != 0) {
        if (func_089EC1CC(record) == 1 || func_089EC210(record) == 1) {
            busy = 1;
            break;
        }
    }
    if (func_089EB9CC(item->indicator, busy) != 0) {
        return -1;
    }
    return 0;
}

}
