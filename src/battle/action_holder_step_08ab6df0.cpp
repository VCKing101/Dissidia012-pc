#include "battle/agent15_types.h"

extern "C" {

extern void func_08AB6E40(ActionHolder *self);
extern void func_08AB6E5C(ActionHolder *self);

void func_08AB6DF0(ActionHolder *self)
{
    switch (self->state) {
    case 2:
        func_08AB6E40(self);
        break;
    case 3:
        func_08AB6E5C(self);
        break;
    }
}

}
