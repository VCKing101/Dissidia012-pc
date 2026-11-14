/* Shows prompt 5 when any save slot is in use (4 otherwise); returns
   whether one is. */
#include "menu/agent24_types.h"

extern void func_088D484C(char *prompt, int id, int flags);

int func_08B2D9FC(A24SaveOwner *self)
{
    A24SaveState *state = self->field_844;
    int used = 0;
    int i;
    for (i = 0; i < 5; i++) {
        if (state->field_64C[i].field_0 != 0) {
            used = 1;
        }
    }
    if (used) {
        func_088D484C(state->field_674, 5, 0);
    } else {
        func_088D484C(state->field_674, 4, 0);
    }
    return used;
}
