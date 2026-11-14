#include "main/auto_region4.h"

/* Obj8F2B84: clears the counters and the four tables. */
void func_088F2C28(Obj8F2B84 *self)
{
    int i;

    self->field_1BBC = 0;
    self->field_1BC0 = 0;
    self->field_1BBD = 0;
    self->field_1BC2 = 0;
    self->field_1BC4 = 0;
    self->field_1BC8 = 0;
    self->field_1BCC = 0;
    self->field_156C = 0;
    self->field_1570 = 0;
    self->field_1574 = 0;
    self->field_1578 = 0;
    for (i = 0; i < 100; i++) {
        self->field_157C[i] = 0;
    }
    for (i = 0; i < 100; i++) {
        self->field_170C[i] = 0;
    }
    for (i = 0; i < 100; i++) {
        self->field_189C[i] = 0;
    }
    for (i = 0; i < 100; i++) {
        self->field_1A2C[i] = 0;
    }
    func_088F3DD8(self, 0);
    func_088F3E38(self, 0);
}
