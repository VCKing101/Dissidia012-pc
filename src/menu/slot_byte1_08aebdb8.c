/* Returns byte 1 of the record func_088B9044 gives for the menu object's +0x13E8 member. */
#include "menu/agent45_types.h"
extern char *func_088B9044(void *p);
extern A45Sub *D_08B01CF8;
unsigned char func_08AEBDB8(void) {
    char *r = func_088B9044(D_08B01CF8->p140 + 0x13E8);
    return r[1];
}
