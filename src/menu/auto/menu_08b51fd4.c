#include "menu/auto_region4.h"

/* Obj516E4: hands what each part holds back to its owner. */
void func_08B51FD4(Obj516E4 *self)
{
    void *found;
    void *object;
    void *owner;

    found = func_088DE804(self->field_1C);
    if (found != 0) {
        owner = self->field_04;
        object = func_088DE804(self->field_1C);
        func_088825D4(owner, object);
    }
    found = func_088CDD7C(self->field_33C);
    if (found != 0) {
        owner = self->field_04;
        object = func_088CDD7C(self->field_33C);
        func_088825D4(owner, object);
    }
    found = func_088D7684(self->field_C0);
    if (found != 0) {
        owner = self->field_04;
        object = func_088D7684(self->field_C0);
        func_088825D4(owner, object);
    }
    found = func_088CE724(self->field_380);
    if (found != 0) {
        owner = self->field_04;
        object = func_088CE724(self->field_380);
        func_088825D4(owner, object);
    }
}
