#include "menu/agent3_types.h"

extern void func_08A464FC(MenuTextOwner *self, int line, int index);

void func_08A46A44(MenuTextOwner *self) {
    int i;
    for (i = 0; i < 4; i++) {
        int index = self->field_418->field_3FC[i];
        if (index != -1) {
            func_08A464FC(self, i, index);
        }
    }
}
