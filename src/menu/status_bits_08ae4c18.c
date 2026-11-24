/* Builds a status bitmask: 2 when func_08AF9D9C(D_08B01CF8) is zero, plus 1 when not blocked and the looked-up record's byte 3 has bit 0. */
#include "menu/agent45_types.h"
extern A45Sub *D_08B01CF8;
extern int func_08AF9D9C(A45Sub *a);
extern int func_08AF96B0(void);
extern unsigned char *func_088B8FD0(void *p, int a);
unsigned int func_08AE4C18(int a0, int a1) {
    unsigned short m = 0;
    unsigned char *r;
    if (func_08AF9D9C(D_08B01CF8) == 0) {
        m = 2;
    }
    if (func_08AF96B0() != 0) {
        return m;
    }
    r = func_088B8FD0(D_08B01CF8->p140 + 0x13E8, a1);
    if (r != 0) {
        if ((r[3] & 1) != 0) {
            m = m | 1;
        }
    } else {
    }
    return m;
}
