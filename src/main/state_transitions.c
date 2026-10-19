#include "main/state_prefixes.h"

/* 0x08865184 is called by menu 0x089E1930; 0x08865190 by menu
 * 0x089E2E84. The numeric state meanings have not been recovered. */
void func_08865184(MainStatePrefix *self) {
    self->field_0014 = 4;
}
void func_08865190(MainStatePrefix *self) {
    self->field_0014 = 3;
}
/* This two-word initializer is called by main 0x088769CC. */
MainStatePrefix *func_0886519C(MainStatePrefix *self) {
    self->field_0004 = 0;
    self->field_0000 = 0;
    return self;
}
