#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08A90688[];
extern const unsigned char D_08A90650[];
extern void func_08A90144(HandleTaskFrame *self);
}

struct HandleTaskDtor08A900B4 {
    static void destroy(HandleTaskFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A90688;
            func_08A90144(self);
            HandleTaskFrame::destructBase(self, D_08A90650);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A900B4(HandleTaskFrame *self, int flags)
{
    HandleTaskDtor08A900B4::destroy(self, flags);
}
