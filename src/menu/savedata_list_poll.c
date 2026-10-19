#include "menu/savedata_dialog.h"

void func_089DD930(MenuRuntimePrefix *self) {
    switch (func_089C5868()) {
    case 1:
        break;
    case 2:
        func_089C5898(2);
        break;
    case 3:
        func_089C5878();
        break;
    case 4:
        break;
    case 0:
        self->field_01DC->field_0008++;
        break;
    default:
        func_089C5878();
        self->field_01DC->field_0008 = 6;
        break;
    }
}
