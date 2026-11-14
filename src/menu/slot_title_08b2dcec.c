/* Shows the title icon (1 on the back row, 0 otherwise), redraws the slot
   labels and the cursor layer. */
#include "menu/agent24_types.h"

extern unsigned short D_08B2F60C[];
extern void func_088D9F5C(void *part, int id, int show);
extern void func_08B2DD60(A24SaveOwner *self);
extern void func_088D44EC(char *layout, char *cursor, unsigned short *table);

void func_08B2DCEC(A24SaveOwner *self)
{
    if (self->field_844->field_678 == 5) {
        func_088D9F5C(self->field_844->field_02C, 1, 1);
    } else {
        func_088D9F5C(self->field_844->field_02C, 0, 1);
    }
    func_08B2DD60(self);
    func_088D44EC(self->field_844->field_01C, self->field_844->field_49C, D_08B2F60C);
}
