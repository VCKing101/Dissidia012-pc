/* Highlights the current save slot's label (used slots get the brighter
   pair) and shows the arrow; on the back row only the back label. */
#include "menu/agent24_types.h"

extern void func_088D3AE8(char *layout, int slot, int text, int next);
extern void func_088D9CFC(void *part);
extern void func_088D9CD8(void *part);

void func_08B2DD60(A24SaveOwner *self)
{
    if (self->field_844->field_678 == 5) {
        func_088D3AE8(self->field_844->field_01C, 4, 1, 1);
        func_088D9CFC(self->field_844->field_028);
    } else {
        int text;
        int second;
        if (self->field_844->field_64C[self->field_844->field_678].field_0 != 0) {
            text = 4;
            second = 3;
        } else {
            text = 3;
            second = 2;
        }
        func_088D3AE8(self->field_844->field_01C, self->field_844->field_678 * 3 + 5, text, 1);
        func_088D3AE8(self->field_844->field_01C, self->field_844->field_678 * 3 + 5, second, 3);
        func_088D9CD8(self->field_844->field_028);
    }
}
