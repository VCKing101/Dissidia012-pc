#include "menu/auto_region4.h"

void func_08B3BF58(ScreenB41CC0 *self)
{
    int value;
    void *object;
    void *messages;

    func_0892D2C0(self->field_C8->field_1C, self->field_C8->messages, 0xB,
                  ((Global9D712CView4 *)D_089D712C)->field_10CC8);
    object = self->field_C8->field_1C;
    messages = self->field_C8->messages;
    value = func_089203BC(((Global9D712CView4 *)D_089D712C)->field_3F548);
    func_0892D2DC(object, messages, 0xB, value);
}
