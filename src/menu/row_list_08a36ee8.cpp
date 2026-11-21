#include "menu/agent36_types.h"

/* Builds the list of rows to show (at most 33): rows without an id, rows
 * whose id is known, or all of them when func_08A41098 says so. Returns
 * how many, at least 1. */

struct Row36EE8 {
    unsigned short id;
};

extern "C" {
int func_08A3C0A0(void);
Row36EE8 *func_08A3C310(int index);
int func_08A41098(BattleGlobal36 *battle);
}



extern "C" int func_08A36EE8(ClockScreen36 *self)
{
    int count = func_08A3C0A0();
    int n;
    int all;

    if (count > 33) {
        count = 33;
    }
    func_08935828(self->work->list, 0, 0x42);
    n = 0;
    all = func_08A41098(D_08A457EC);
    for (int i = 0; i < count; i++) {
        Row36EE8 *row = func_08A3C310(i);
        int shown = 1;
        if (row->id != 0xFFFF) {
            shown = Known36::Of(row->id);
        }
        if (all) {
            shown = 1;
        }
        if (shown) {
            self->work->list[n] = i;
            n++;
        }
    }
    if (n == 0) {
        n = 1;
    }
    return n;
}
