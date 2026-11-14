#include "menu/agent3_types.h"

extern unsigned short D_08A480E0[];
extern unsigned short D_08A480EC[];
extern void func_088D9F5C(int handle, int text, int mode);
extern void func_088D9CFC(int handle);

void func_08A46238(MenuTextOwner *self, unsigned char mode, int column, int row, int group, int index) {
    int slot = row * 8 + column;
    if (mode == 1) {
        func_088D9F5C(self->field_418->field_04->field_6C[slot], D_08A480E0[group * 3 + index], 1);
    } else {
        func_088D9CFC(self->field_418->field_04->field_6C[slot]);
    }
}

void func_08A462B8(MenuTextOwner *self, int index) {
    func_088D9F5C(self->field_418->field_04->field_DC, D_08A480EC[index], 1);
}
