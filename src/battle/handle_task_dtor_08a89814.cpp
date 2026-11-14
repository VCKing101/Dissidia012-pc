#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08A89D90[];
extern const unsigned char D_08A89D58[];
extern void func_08A898A4(HandleTaskFrame *self);
}

struct HandleTaskDtor08A89814 {
    static void destroy(HandleTaskFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A89D90;
            func_08A898A4(self);
            HandleTaskFrame::destructBase(self, D_08A89D58);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A89814(HandleTaskFrame *self, int flags)
{
    HandleTaskDtor08A89814::destroy(self, flags);
}
