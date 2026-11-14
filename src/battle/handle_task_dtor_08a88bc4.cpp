#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08A891C8[];
extern const unsigned char D_08A89190[];
extern void func_08A88C58(HandleTaskFrame *self);
}

struct HandleTaskDtor08A88BC4 {
    static void destroy(HandleTaskFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A891C8;
            func_08A88C58(self);
            HandleTaskFrame::destructBase(self, D_08A89190);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A88BC4(HandleTaskFrame *self, int flags)
{
    HandleTaskDtor08A88BC4::destroy(self, flags);
}
