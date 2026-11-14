#include "battle/auto_region4.h"

typedef struct HandlesCF9C0 {
    char pad_00[0x70];
    void *handles[6];
} HandlesCF9C0;

/* Frees the six handles at field_70. */
void func_08ACF9C0(HandlesCF9C0 *self)
{
    int i;

    for (i = 0; i < 6; i++) {
        if (self->handles[i] != 0) {
            func_088A6C70(self->handles[i]);
        }
        self->handles[i] = 0;
    }
}
