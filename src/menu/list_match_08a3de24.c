#include "menu/agent3_types.h"

extern int func_08A3DD10(MenuListOwner *self, MenuListItem *item, int key);

int func_08A3DE24(MenuListOwner *self, int key) {
    int i;
    for (i = 0; i < self->field_2A14->field_29F4; i++) {
        if (func_08A3DD10(self, &self->field_2A14->field_4B8[i], key) != 0) {
            return 1;
        }
    }
    return 0;
}
