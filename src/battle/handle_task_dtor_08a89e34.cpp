#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08A8A2C0[];
extern const unsigned char D_08A8A288[];
extern void func_08A89EB4(HandleTaskFrame *self);
}

struct HandleTaskDtor08A89E34 {
    static void destroy(HandleTaskFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A8A2C0;
            func_08A89EB4(self);
            HandleTaskFrame::destructBase(self, D_08A8A288);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A89E34(HandleTaskFrame *self, int flags)
{
    HandleTaskDtor08A89E34::destroy(self, flags);
}
