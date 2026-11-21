#include "menu/agent36_types.h"

/* Name list screen: builds the list of names to show (at most 0x40): known
 * ones, all when func_08A430B8 says so, or those func_08A3DE24 accepts.
 * Returns how many, at least 1. */

struct Row3DEB8 {
    unsigned short text;
    unsigned short id;        /* +0x02 */
    char pad_04[0x4];
    unsigned short kind;      /* +0x08 */
};

extern "C" {
int func_08A3C034(void);
int func_08A430B8(void);
Row3DEB8 *func_08A3C210(int index);
int func_08A3DE24(NameScreen36 *self, int kind);
}

extern "C" int func_08A3DEB8(NameScreen36 *self)
{
    int count;
    int n;
    int all;

    func_08935828(self->work->ids, 0, 0x80);
    count = func_08A3C034();
    if (count > 0x40) {
        count = 0x40;
    }
    n = 0;
    all = func_08A430B8();
    for (int i = 0; i < count; i++) {
        Row3DEB8 *row = func_08A3C210(i);
        int shown = 0;
        if (row->id != 0xFFFF) {
            shown = Known36::Of(row->id);
        }
        if (all) {
            shown = 1;
        }
        if (shown || func_08A3DE24(self, row->kind) != 0) {
            self->work->ids[n] = i;
            n++;
        }
    }
    if (n == 0) {
        n = 1;
    }
    return n;
}
