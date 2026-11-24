/* Returns bit 0 of byte 3 of the record func_088B9044 gives for the menu object's +0x13E8 member. */
#include "menu/agent45_types.h"
extern unsigned char *func_088B9044(void *p);
extern A45Sub *D_08B01CF8;
unsigned char func_08AEBE18(void) {
    unsigned char *r = func_088B9044(D_08B01CF8->p140 + 0x13E8);
    int t = 0;
    if (r[3] & 1) t = 1;
    return (unsigned char)t;
}
