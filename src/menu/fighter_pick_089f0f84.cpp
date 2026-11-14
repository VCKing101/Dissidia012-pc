#include "menu/agent29_types.h"

/* Screen0B6A0: picks a fighter for a pull: a fixed one (`kind` > 0) when
   func_089F0BF8 allows it, else one at random from a group of the
   fighter table D_08A0B2EC (-2, -3, -5, -6 pick ranges, others all of
   it) among those allowed; -1 when none is. */
extern "C" {
extern int D_09B6DA54;
extern const unsigned short D_08A0B2EC[];
int func_089F0BF8(Screen0B6A0 *self, signed char fighter);
}

struct Random29 {
    static int Next() {
        D_09B6DA54 = D_09B6DA54 * 0x343FD + 0x269EC3;
        return (D_09B6DA54 >> 16) & 0x7FFF;
    }
    static int Range(int low, int high) {
        return Next() % (high - low + 1) + low;
    }
};

extern "C" int func_089F0F84(Screen0B6A0 *self, short kind)
{
    signed char allowed[0x24];
    int result = -1;
    int count;
    int first;
    int last;
    int i;

    if (kind > 0) {
        if (func_089F0BF8(self, kind) != 0) {
            result = (signed char)kind;
        }
    } else {
        count = 0;
        if (kind == -2) {
            first = 0;
            last = 0x11;
        } else if (kind == -3) {
            first = 0x12;
            last = 0x1D;
        } else if (kind == -5) {
            first = 0xB;
            last = 0x10;
        } else if (kind == -6) {
            first = 1;
            last = 0xA;
        } else {
            first = 0;
            last = 0x1D;
        }
        for (i = first; i <= last; i++) {
            if (func_089F0BF8(self, D_08A0B2EC[i]) != 0) {
                allowed[count] = D_08A0B2EC[i];
                count++;
            }
        }
        if (count != 0) {
            result = allowed[Random29::Range(0, count - 1)];
        }
    }
    return result;
}
