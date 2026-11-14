#include "battle/agent15_types.h"

extern "C" {

extern void func_08AB7044(ActionHolder *self);

void func_08AB7024(ActionHolder *self, unsigned char flag)
{
    self->flag_565 = flag;
    func_08AB7044(self);
}

}
