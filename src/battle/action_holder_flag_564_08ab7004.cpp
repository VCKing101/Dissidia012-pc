#include "battle/agent15_types.h"

extern "C" {

extern void func_08AB7044(ActionHolder *self);

void func_08AB7004(ActionHolder *self, unsigned char flag)
{
    self->flag_564 = flag;
    func_08AB7044(self);
}

}
