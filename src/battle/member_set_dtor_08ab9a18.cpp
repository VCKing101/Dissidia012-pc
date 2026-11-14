#include "battle/agent14_types.h"

/* Holder of seven small objects (vtable at 8 each) with its own vtable after them. */
struct MemberSetFrame {
    unsigned char active;
    void *handle;
    TableAt08Object member_08;
    TableAt08Object member_14;
    TableAt08Object member_20;
    TableAt08Object member_2C;
    TableAt08Object member_38;
    TableAt08Object member_44;
    TableAt08Object member_50;
    const void *vtable;
};

extern "C" {
extern const unsigned char D_08AB9F28[];
extern const unsigned char D_08AB9F38[];
extern const unsigned char D_08AB9F48[];
extern const unsigned char D_08AB9F58[];
extern const unsigned char D_08AB9F68[];
extern const unsigned char D_08AB9F78[];
extern const unsigned char D_08AB9F88[];
extern const unsigned char D_08AB9F98[];
extern void func_08AB9B24(MemberSetFrame *self);
}

struct MemberSetDtor {
    static void destroy(MemberSetFrame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08AB9F28;
            func_08AB9B24(self);
            TableAt08Object::destroy(&self->member_50, 0, D_08AB9F98);
            TableAt08Object::destroy(&self->member_44, 0, D_08AB9F88);
            TableAt08Object::destroy(&self->member_38, 0, D_08AB9F78);
            TableAt08Object::destroy(&self->member_2C, 0, D_08AB9F68);
            TableAt08Object::destroy(&self->member_20, 0, D_08AB9F58);
            TableAt08Object::destroy(&self->member_14, 0, D_08AB9F48);
            TableAt08Object::destroy(&self->member_08, 0, D_08AB9F38);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08AB9A18(MemberSetFrame *self, int flags)
{
    MemberSetDtor::destroy(self, flags);
}
