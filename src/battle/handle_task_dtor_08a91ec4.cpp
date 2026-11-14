#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08A92330[];
extern const unsigned char D_08A922F8[];
extern void func_08A91F68(HandleTaskFrame *self);
}

struct HandleTaskDtor08A91EC4 {
    static void destroy(HandleTaskFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A92330;
            func_08A91F68(self);
            HandleTaskFrame::destructBase(self, D_08A922F8);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A91EC4(HandleTaskFrame *self, int flags)
{
    HandleTaskDtor08A91EC4::destroy(self, flags);
}
