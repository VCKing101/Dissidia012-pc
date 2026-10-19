#include "main/resource_descriptor.h"

void func_088CBA10(MainDescriptorPrefix *self);

void *func_088CB9E8(void *object) {
    func_088CBA10(object);
    return object;
}

void func_088CBA10(MainDescriptorPrefix *self) {
    self->field_0000 = -1;
    self->field_0001 = 1;
    self->field_0002 = 0;
    self->field_0004 = 0;
    self->field_0003 = 0;
    self->field_000C = 0;
    self->field_0006 = 0;
    self->field_0008 = 0;
    func_088B7E30(self->subobject_0010);
}

void func_088CBA54(MainDescriptorPrefix *self, int value) {
    self->field_0000 = value;
    func_088B83F0(self->subobject_0010);
}

