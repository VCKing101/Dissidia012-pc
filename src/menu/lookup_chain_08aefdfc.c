/* Looks up a record via func_088B70C8 and, if found, returns func_088C09FC of its byte 1; else 0. */
#include "menu/agent45_types.h"
extern unsigned char *func_088B70C8(void *p);
extern int func_088C09FC(int v);
int func_08AEFDFC(A45Root *self) {
    unsigned char *r = func_088B70C8(*(void **)self->p4->p140);
    int res = 0;
    if (r != 0) res = func_088C09FC(r[1]);
    return res;
}
