#include "battle/agent14_types.h"

/* Holder of six small objects (vtable at 8 each) with its own vtable after them. */
struct MemberSet6Frame {
    unsigned char active;
    void *handle;
    TableAt08Object member_08;
    TableAt08Object member_14;
    TableAt08Object member_20;
    TableAt08Object member_2C;
    TableAt08Object member_38;
    TableAt08Object member_44;
    const void *vtable;
};

extern "C" {
extern const unsigned char D_08ABC580[];
extern const unsigned char D_08ABC590[];
extern const unsigned char D_08ABC5A0[];
extern const unsigned char D_08ABC5B0[];
extern const unsigned char D_08ABC5C0[];
extern const unsigned char D_08ABC5D0[];
extern const unsigned char D_08ABC5E0[];
extern void func_08ABC0DC(MemberSet6Frame *self);
}

struct MemberSet6Dtor {
    static void destroy(MemberSet6Frame *self, int flags)
    {
        if (self != 0) {
            self->vtable = D_08ABC580;
            func_08ABC0DC(self);
            TableAt08Object::destroy(&self->member_44, 0, D_08ABC5E0);
            TableAt08Object::destroy(&self->member_38, 0, D_08ABC5D0);
            TableAt08Object::destroy(&self->member_2C, 0, D_08ABC5C0);
            TableAt08Object::destroy(&self->member_20, 0, D_08ABC5B0);
            TableAt08Object::destroy(&self->member_14, 0, D_08ABC5A0);
            TableAt08Object::destroy(&self->member_08, 0, D_08ABC590);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" void func_08ABBFE8(MemberSet6Frame *self, int flags)
{
    MemberSet6Dtor::destroy(self, flags);
}
