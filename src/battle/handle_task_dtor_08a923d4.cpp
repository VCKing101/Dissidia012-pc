#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08A92890[];
extern const unsigned char D_08A92858[];
extern void func_08A92454(HandleTaskFrame *self);
}

struct HandleTaskDtor08A923D4 {
    static void destroy(HandleTaskFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A92890;
            func_08A92454(self);
            HandleTaskFrame::destructBase(self, D_08A92858);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A923D4(HandleTaskFrame *self, int flags)
{
    HandleTaskDtor08A923D4::destroy(self, flags);
}
