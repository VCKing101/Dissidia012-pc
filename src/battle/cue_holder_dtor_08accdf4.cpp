#include "battle/agent14_types.h"

/* Object holding one small member (vtable at 0x10 of it) at 0xC; no vtable of its own. */
struct CueHolderFrame {
    char data_00[0xC];
    TableAt10Object member_0C;
};

extern "C" {
extern const unsigned char D_08ACD0C8[];
extern void func_08ACCE68(CueHolderFrame *self);
}

struct CueHolderDtor {
    static void destroy(CueHolderFrame *self, int flags)
    {
        if (self != 0) {
            func_08ACCE68(self);
            TableAt10Object::destroy(&self->member_0C, 0, D_08ACD0C8);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08ACCDF4(CueHolderFrame *self, int flags)
{
    CueHolderDtor::destroy(self, flags);
}
