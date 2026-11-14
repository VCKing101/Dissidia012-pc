#include "battle/agent28_types.h"

/* Shows the card item while either of its two show requests is set; on
 * showing, restarts every active record and the indicator. */
struct CardList {
    static CardRecord *next(CardItemState *item, CardRecord *record)
    {
        return !record ? item->head : record->next;
    }
};

extern "C" {

extern void func_089EC114(CardRecord *record);
extern void func_089EBB48(void *indicator);

int func_089EB5F8(CardItemState *item)
{
    unsigned char shown = 0;
    for (int request = 0; request < 2; request++) {
        if (item->flags[request] == 1) {
            shown = 1;
            break;
        }
    }
    if (shown != item->shown) {
        if (shown == 1) {
            CardRecord *record = 0;
            while ((record = CardList::next(item, record)) != 0) {
                func_089EC114(record);
            }
            func_089EBB48(item->indicator);
        }
        item->shown = shown;
    }
    return 0;
}

}
