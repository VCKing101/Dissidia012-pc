#include "menu/resource_phases.h"

void func_089DDCC4(MenuRuntimePrefix *self) {
    if (self->field_01DC->field_004A) {
        int step = func_08867888() != 6 ? 11 : 9;
        self->field_01DC->field_0008 = step;
    }
}
