#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08A89768[];
extern const unsigned char D_08A89730[];
extern void func_08A892F0(HandleTaskFrame *self);
}

struct HandleTaskDtor08A8926C {
    static void destroy(HandleTaskFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A89768;
            func_08A892F0(self);
            HandleTaskFrame::destructBase(self, D_08A89730);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A8926C(HandleTaskFrame *self, int flags)
{
    HandleTaskDtor08A8926C::destroy(self, flags);
}
