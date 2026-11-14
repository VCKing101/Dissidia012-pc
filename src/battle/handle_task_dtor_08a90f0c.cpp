#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08A91508[];
extern const unsigned char D_08A914D0[];
extern void func_08A90F9C(HandleTaskFrame *self);
}

struct HandleTaskDtor08A90F0C {
    static void destroy(HandleTaskFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A91508;
            func_08A90F9C(self);
            HandleTaskFrame::destructBase(self, D_08A914D0);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A90F0C(HandleTaskFrame *self, int flags)
{
    HandleTaskDtor08A90F0C::destroy(self, flags);
}
