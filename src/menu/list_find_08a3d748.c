#include "menu/agent3_types.h"

int func_08A3D748(MenuListOwner *self, int key) {
    int i;
    for (i = 0; i < self->field_2A14->field_2A00; i++) {
        if (self->field_2A14->field_2838[i].field_00 == key) {
            return 1;
        }
    }
    return 0;
}

void func_08A3F518(MenuListOwner *self, MenuListItem *item);

void func_08A3D790(MenuListOwner *self, int value) {
    MenuListItem *item = &self->field_2A14->field_4B8[self->field_2A14->field_29F4];
    item->field_00 = value;
    item->field_08 = 2;
    func_08A3F518(self, item);
    self->field_2A14->field_29F4++;
}
