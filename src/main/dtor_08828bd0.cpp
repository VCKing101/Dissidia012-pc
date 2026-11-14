#include "main/agent4_types.h"

extern "C" {
extern int D_089C78C0;
extern int D_089C78D0;
void func_08828C50(void *self);
}

struct Agent4Owner28BD0 {
    int field_00;
    int field_04;
    int field_08;
    Agent4Vptr08 member;
    void *vtable;

    static void destroy(Agent4Owner28BD0 *self, unsigned int flags) {
        if (self != 0) {
            self->vtable = &D_089C78C0;
            func_08828C50(self);
            Agent4Vptr08::destroy(&self->member, 0, &D_089C78D0);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {
/* Deleting destructor; vtable 0x089C78C0. */
void func_08828BD0(Agent4Owner28BD0 *self, unsigned int flags) {
    Agent4Owner28BD0::destroy(self, flags);
}
}
