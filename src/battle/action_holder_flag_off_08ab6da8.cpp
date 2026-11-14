#include "battle/agent15_types.h"

extern "C" {

extern void func_08AB7024(ActionHolder *self, unsigned char flag);

int func_08AB6DA8(ActionHolder *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->state == 0) {
        return 0;
    }
    func_08AB7024(self, 0);
    return 0;
}

}
