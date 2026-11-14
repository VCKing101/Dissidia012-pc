#include "battle/agent28_types.h"

/* Starts the first active record of the given kind that is not already
 * running, fading or finished. */
struct CardList {
    static CardRecord *next(CardItemState *item, CardRecord *record)
    {
        return !record ? item->head : record->next;
    }
};

extern "C" {

extern int func_089EC2D8(CardRecord *record);
extern int func_089EC238(CardRecord *record);
extern int func_089EC260(CardRecord *record);
extern int func_089EC288(CardRecord *record);
extern int func_089EC0EC(CardRecord *record);

int func_089EAB30(CardItemState *item, int kind)
{
    if (!item->open) {
        return -1;
    }
    if (kind < 0) {
        return -2;
    }
    CardRecord *record = 0;
    CardRecord *found;
    while ((record = found = CardList::next(item, record)) != 0) {
        if (func_089EC2D8(record) == kind && func_089EC238(record) != 1 && func_089EC260(record) != 1 &&
            func_089EC288(record) != 1) {
            break;
        }
    }
    if (found != 0) {
        if (func_089EC0EC(record) != 0) {
            return -3;
        }
    }
    return 0;
}

}
