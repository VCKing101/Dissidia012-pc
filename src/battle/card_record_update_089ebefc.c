#include "battle/agent28_types.h"

/* Runs the card record's current state, then places it in its slot. */
extern void func_089ECB50(CardRecord *record);
extern int func_089EC358(CardRecord *record);
extern int func_089EC4D8(CardRecord *record);
extern int func_089EC5F8(CardRecord *record);
extern int func_089EC73C(CardRecord *record);
extern void func_089EC760(CardRecord *record, int slot);
extern void func_089EC7F8(CardRecord *record, int x, int y, int slot);

int func_089EBEFC(CardRecord *record, int x, int y, int slot)
{
    if (!record->open) {
        return -1;
    }
    if (slot < 0) {
        return -2;
    }
    switch (record->state) {
    case 0:
        func_089ECB50(record);
        break;
    case 1:
        func_089EC358(record);
        break;
    case 2:
        func_089EC4D8(record);
        break;
    case 3:
        func_089EC5F8(record);
        break;
    case 4:
        func_089EC73C(record);
        break;
    }
    func_089EC760(record, slot);
    func_089EC7F8(record, x, y, slot);
    return 0;
}
