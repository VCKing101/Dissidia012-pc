#include "main/resource_descriptor.h"

void *func_0889F63C(void *object) {
    MainResourceHandlePrefix *self = object;
    self->field_0000 = -1;
    self->field_0004 = 0;
    self->field_0008 = 1.0f;
    self->field_000C = 1.0f;
    self->field_0010 = 0.0f;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001C = 0;
    return self;
}

void func_0889F674(void *object, unsigned int flags) {
    if (object && (flags & 1)) func_08992F04(object);
}

int func_0889F6A0(MainResourceHandlePrefix *self, int a, unsigned int b, unsigned int c) {
    if (self->field_0000 != -1) return 0;
    self->field_0000 = a;
    return func_088A1380(D_089D6C88, a, b, c, 0);
}

int func_0889F704(MainResourceHandlePrefix *self) {
    if (self->field_0000 != -1) {
        int result = func_088A13E4(D_089D6C88, self->field_0000);
        self->field_0000 = -1;
        return result;
    }
    return 0;
}
