#include "main/auto_region4.h"

/* Obj9D7E88: hands the objects of field_1874 and field_1AF0 to the owner at +4. */
void func_088E9474(Obj9D7E88 *self)
{
    void *result;
    void *object;
    void *owner;

    result = func_088D7684(self->field_1874);
    if (result != 0) {
        owner = self->callbacks.field_00;
        object = func_088D7684(self->field_1874);
        func_088825D4(owner, object);
    }
    result = func_088CE724(self->field_1AF0);
    if (result != 0) {
        owner = self->callbacks.field_00;
        object = func_088CE724(self->field_1AF0);
        func_088825D4(owner, object);
    }
}
