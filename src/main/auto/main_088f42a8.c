#include "main/auto_region4.h"

/* Calls func_088F414C for the ids 1..0x20 (all but 0x17). */
void func_088F42A8(void *self)
{
    int id;

    id = 1;
    do {
        if (id != 0x17) {
            func_088F414C(self, id, 1);
        }
        id = id + 1;
    } while (id < 0x21);
}
