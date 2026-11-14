#include "menu/agent3_types.h"

extern unsigned short D_08A48100[];
extern void func_088D9F5C(int handle, int text, int mode);
extern void func_088D9CFC(int handle);

void func_08A46330(MenuTextOwner *self, unsigned char mode, int slot, int index) {
    if (mode == 1) {
        func_088D9F5C(self->field_418->field_04->field_CC[slot], D_08A48100[index], 1);
    } else {
        func_088D9CFC(self->field_418->field_04->field_CC[slot]);
    }
}
