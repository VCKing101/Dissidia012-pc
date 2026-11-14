#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08A90000[];
extern const unsigned char D_08A8FFC8[];
extern void func_08A8F8A0(HandleTaskFrame *self);
}

struct HandleTaskDtor08A8F7E4 {
    static void destroy(HandleTaskFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A90000;
            func_08A8F8A0(self);
            HandleTaskFrame::destructBase(self, D_08A8FFC8);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A8F7E4(HandleTaskFrame *self, int flags)
{
    HandleTaskDtor08A8F7E4::destroy(self, flags);
}
