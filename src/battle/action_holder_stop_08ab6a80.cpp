#include "battle/agent15_types.h"

extern "C" {

extern void func_08AB7004(ActionHolder *self, unsigned char flag);

int func_08AB6A80(ActionHolder *self)
{
    if (self->active == 0) {
        return -1;
    }
    self->slot_160();
    func_08AB7004(self, 0);
    self->state = 0;
    return 0;
}

}
