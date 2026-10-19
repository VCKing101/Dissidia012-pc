/* Uncredited probe: branch direction and return-block order differ. */
#include "main/drawing_record.h"
extern unsigned int func_088D991C(MainDrawingOwnerPrefix *self);

int func_088D9924(MainDrawingOwnerPrefix *self) {
    if (func_088D991C(self)) {
        if (!self->field_0004) {
            self->field_0004 = func_088DAA40(self->field_0008, 12, 0);
            if (!self->field_0004) goto allocation_failed;
        }
        return 0;
    }
    return -1;
allocation_failed:
    return -2;
}
