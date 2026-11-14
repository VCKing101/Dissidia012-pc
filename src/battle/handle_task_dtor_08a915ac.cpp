#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08A91A50[];
extern const unsigned char D_08A91A18[];
extern void func_08A91634(HandleTaskFrame *self);
}

struct HandleTaskDtor08A915AC {
    static void destroy(HandleTaskFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A91A50;
            func_08A91634(self);
            HandleTaskFrame::destructBase(self, D_08A91A18);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A915AC(HandleTaskFrame *self, int flags)
{
    HandleTaskDtor08A915AC::destroy(self, flags);
}
