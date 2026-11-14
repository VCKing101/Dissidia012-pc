#include "battle/agent28_types.h"

/* Ends the second item's finished records, then lays out the active records
 * from the tail, queued ones first, numbering the ones still waiting. */
struct CardList {
    static CardRecord *next(CardItemState *item, CardRecord *record)
    {
        return !record ? item->head : record->next;
    }
};

extern "C" {

extern int func_089EC1CC(CardRecord *record);
extern int func_089EC210(CardRecord *record);
extern int func_089EC0EC(CardRecord *record);
extern int func_089EC318(CardRecord *record);
extern void func_089EBEFC(CardRecord *record, int x, int y, int slot);
extern int func_089EC188(CardRecord *record);
extern int func_089EC2B0(CardRecord *record);
extern int func_089EC260(CardRecord *record);
extern int func_089EC288(CardRecord *record);

int func_089EB3E8(CardItemState *item, int x, int y)
{
    if (item->index == 1) {
        CardRecord *record = 0;
        while ((record = CardList::next(item, record)) != 0) {
            if (func_089EC1CC(record) == 1 || func_089EC210(record) == 1) {
                func_089EC0EC(record);
            }
        }
    }
    unsigned char order[2] = { 1, 0 };
    int slot = 0;
    for (int pass = 0; pass < 2; pass++) {
        for (CardRecord *record = item->tail; record != 0; record = record->prev) {
            if (func_089EC318(record) == order[pass]) {
                func_089EBEFC(record, x, y, slot);
                if (func_089EC188(record) != 1 && func_089EC2B0(record) != 1 && func_089EC260(record) != 1 &&
                    func_089EC288(record) != 1) {
                    slot++;
                }
            }
        }
    }
    return 0;
}

}
