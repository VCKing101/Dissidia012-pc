#include "battle/agent14_types.h"

extern "C" {
extern const unsigned char D_08A91E20[];
extern const unsigned char D_08A91DE8[];
extern void func_08A91B74(HandleTaskFrame *self);
}

struct HandleTaskDtor08A91AF4 {
    static void destroy(HandleTaskFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A91E20;
            func_08A91B74(self);
            HandleTaskFrame::destructBase(self, D_08A91DE8);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A91AF4(HandleTaskFrame *self, int flags)
{
    HandleTaskDtor08A91AF4::destroy(self, flags);
}
