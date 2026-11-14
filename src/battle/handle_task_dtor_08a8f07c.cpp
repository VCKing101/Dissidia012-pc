#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08A8F738[];
extern const unsigned char D_08A8F700[];
extern void func_08A8F108(HandleTaskFrame *self);
}

struct HandleTaskDtor08A8F07C {
    static void destroy(HandleTaskFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A8F738;
            func_08A8F108(self);
            HandleTaskFrame::destructBase(self, D_08A8F700);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A8F07C(HandleTaskFrame *self, int flags)
{
    HandleTaskDtor08A8F07C::destroy(self, flags);
}
