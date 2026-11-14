#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08A941F0[];
extern const unsigned char D_08A941B8[];
extern void func_08A929B0(HandleTaskFrame *self);
}

struct HandleTaskDtor08A92934 {
    static void destroy(HandleTaskFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A941F0;
            func_08A929B0(self);
            HandleTaskFrame::destructBase(self, D_08A941B8);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A92934(HandleTaskFrame *self, int flags)
{
    HandleTaskDtor08A92934::destroy(self, flags);
}
