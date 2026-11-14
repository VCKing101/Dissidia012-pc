#include "menu/agent3_types.h"

extern unsigned short D_08A480CA[];
extern unsigned short D_08A480DA[];
extern unsigned short D_08A480F0[];
extern void func_088D3AE8(MenuTextPanel *panel, int a, int b, int c);

void func_08A464B0(MenuTextOwner *self, int first, int second) {
    func_088D3AE8(self->field_418->field_04, D_08A480CA[first], D_08A480DA[second], 0x38);
}

void func_08A464FC(MenuTextOwner *self, int line, int index) {
    func_088D3AE8(self->field_418->field_04, D_08A480F0[index], 0, line + 0x33);
}
