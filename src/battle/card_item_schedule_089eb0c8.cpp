#include "battle/agent28_types.h"

/* Readies the first item's held records, then, unless the battle is paused,
 * starts waiting records up to the limit: queued ones first, then the rest. */
struct CardList {
    static CardRecord *next(CardItemState *item, CardRecord *record)
    {
        return !record ? item->head : record->next;
    }
};

extern "C" {

extern int func_089EC2B0(CardRecord *record);
extern void func_089EC164(CardRecord *record);
extern int func_089E9B18(void);
extern int func_089EB6AC(CardItemState *item);
extern int func_089EB6D8(CardItemState *item);
extern int func_089EC188(CardRecord *record);
extern int func_089EC318(CardRecord *record);
extern int func_089EC04C(CardRecord *record);

int func_089EB0C8(CardItemState *item)
{
    if (item->index == 0) {
        CardRecord *record = 0;
        while ((record = CardList::next(item, record)) != 0) {
            if (func_089EC2B0(record) == 1) {
                func_089EC164(record);
            }
        }
    }
    if (func_089E9B18() == 0) {
        int limit = func_089EB6AC(item);
        int running = func_089EB6D8(item);
        for (int pass = 0; pass < 2; pass++) {
            if (running >= limit) {
                break;
            }
            CardRecord *record = 0;
            int queued = pass == 0;
            while ((record = CardList::next(item, record)) != 0) {
                if (running >= limit) {
                    break;
                }
                if (func_089EC188(record) == 0) {
                    continue;
                }
                if (func_089EC318(record) != queued) {
                    continue;
                }
                if (func_089EC04C(record) != 0) {
                    return -1;
                }
                running++;
            }
        }
    }
    return 0;
}

}
