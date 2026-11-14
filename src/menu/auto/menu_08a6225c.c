#include "menu/auto_region2.h"

/* Panel63680: picks field_D34 from kind (0, kind * 2, or 0xB above 5) and redraws, state 5. */
void func_08A6225C(Panel63680 *self, int kind, unsigned char flag)
{
    if (kind == 0) {
        self->field_D34 = 0;
        func_08A629BC(self);
    } else if (kind < 6) {
        self->field_D34 = kind * 2;
        func_08A629BC(self);
        self->field_D60 = flag == 0;
    } else {
        self->field_D34 = 0xB;
        func_08A629BC(self);
        self->field_D60 = flag == 0;
    }
    func_08A62AFC(self);
    func_08A62C58(self);
    func_08A63024(self);
    func_08A621C8(self);
}
