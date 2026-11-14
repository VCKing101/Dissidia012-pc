#include "battle/agent14_types.h"

struct TrackTaskFrame {
    const void *vtable;
    char pad_004[0x30];
    char member_034[0x4];
};

extern "C" {
extern const unsigned char D_08A90E60[];
extern const unsigned char D_08A90E28[];
extern void func_08A907EC(TrackTaskFrame *self);
extern void func_08A821A0(void *member, int flags);
}

struct TrackTaskDtor {
    static void destroy(TrackTaskFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A90E60;
            func_08A907EC(self);
            func_08A821A0(self->member_034, 2);
            HandleTaskFrame::destructBase((HandleTaskFrame *)self, D_08A90E28);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A90734(TrackTaskFrame *self, int flags)
{
    TrackTaskDtor::destroy(self, flags);
}
