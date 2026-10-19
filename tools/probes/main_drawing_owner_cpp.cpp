extern "C" {
#include "main/drawing_record.h"
unsigned int func_088D991C(MainDrawingOwnerPrefix *);
}
extern "C" int func_088D9924(MainDrawingOwnerPrefix *self) {
    if (func_088D991C(self)) {
        if (!self->field_0004) self->field_0004 = func_088DAA40(self->field_0008,12,0);
        if (self->field_0004) return 0;
        else return -2;
    } else return -1;
}
