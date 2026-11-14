#include "battle/auto_region3.h"

/* Frees the nine handles at field_228. */
void func_08A3DB7C(Model3E874 *self)
{
    int i;

    for (i = 0; i < 9; i++) {
        if (self->field_228[i] != 0) {
            func_088DA060(self->field_228[i]);
        }
        self->field_228[i] = 0;
    }
}
