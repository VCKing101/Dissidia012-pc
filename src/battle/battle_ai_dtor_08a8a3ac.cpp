#include "battle/agent14_types.h"

/* AI task with nine member subobjects; the vtable pointer is at 0. */
struct BattleAiFrame {
    const void *vtable;
    char pad_004[0x38];
    char member_03C[0x10];
    char member_04C[0x28];
    char member_074[0x14];
    char member_088[0x94];
    char member_11C[0x48];
    char member_164[0x1C];
    char member_180[0x10];
    char member_190[0x30];
    char member_1C0[0x4];
};

extern "C" {
extern const unsigned char D_08A8EFC0[];
extern const unsigned char D_08A8EF88[];
extern void func_08A8A4B0(BattleAiFrame *self);
extern void func_08A85C38(void *member, int flags);
extern void func_08A94260(void *member, int flags);
extern void func_08A821A0(void *member, int flags);
extern void func_08A94E18(void *member, int flags);
extern void func_08AA5858(void *member, int flags);
extern void func_08A980C8(void *member, int flags);
extern void func_08A81610(void *member, int flags);
extern void func_08AA42A8(void *member, int flags);
extern void func_08A7E778(void *member, int flags);
}

struct BattleAiDtor {
    static void destroy(BattleAiFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08A8EFC0;
            func_08A8A4B0(self);
            func_08A85C38(self->member_1C0, 2);
            func_08A94260(self->member_190, 2);
            func_08A821A0(self->member_180, 2);
            func_08A94E18(self->member_164, 2);
            func_08AA5858(self->member_11C, 2);
            func_08A980C8(self->member_088, 2);
            func_08A81610(self->member_074, 2);
            func_08AA42A8(self->member_04C, 2);
            func_08A7E778(self->member_03C, 2);
            HandleTaskFrame::destructBase((HandleTaskFrame *)self, D_08A8EF88);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08A8A3AC(BattleAiFrame *self, int flags)
{
    BattleAiDtor::destroy(self, flags);
}
