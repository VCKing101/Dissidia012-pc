#include "main/agent4_types.h"

extern "C" {
extern int D_089C6D60;
extern int D_089C6D70;
extern int D_089C6D80;
void func_088105E0(void *self);
}

/* Member with its own vtable pointer at +0x08; members are destroyed with flags 0. */
struct Agent4Member08 {
    int field_00;
    int field_04;
    void *vtable;

    static void destroy(Agent4Member08 *self, unsigned int flags, void *vtable) {
        if (self != 0) {
            self->vtable = vtable;
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

struct Agent4MemberPair {
    int field_00;
    Agent4Member08 first;
    Agent4Member08 second;

    static void destroy(Agent4MemberPair *self, unsigned int flags) {
        if (self != 0) {
            Agent4Member08::destroy(&self->second, 0, &D_089C6D80);
            Agent4Member08::destroy(&self->first, 0, &D_089C6D70);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

struct Agent4Owner10520 {
    int field_00;
    int field_04;
    Agent4MemberPair members;
    void *vtable;

    static void destroy(Agent4Owner10520 *self, unsigned int flags) {
        if (self != 0) {
            self->vtable = &D_089C6D60;
            func_088105E0(self);
            Agent4MemberPair::destroy(&self->members, 0);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {
/* Deleting destructor; vtable 0x089C6D60. */
void func_08810520(Agent4Owner10520 *self, unsigned int flags) {
    Agent4Owner10520::destroy(self, flags);
}
}
