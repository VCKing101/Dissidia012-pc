/* These return reviewed data-region addresses. Original types are unknown. */
#include "menu/configuration.h"

extern unsigned char D_08A9443C[];
extern unsigned char D_08A94638[];
extern unsigned char D_08A94568[];
void *func_08A93DC4(void) {
    return D_08A9443C;
}
void *func_08A93DD0(void) {
    return D_08A94638;
}
void *func_08A93DDC(void) {
    return &D_08A94670;
}
void *func_08A93DE8(void) {
    return D_08A94568;
}
