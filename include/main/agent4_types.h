#ifndef MAIN_AGENT4_TYPES_H
#define MAIN_AGENT4_TYPES_H

/* Shared shapes recovered by agent4 (main executable). */

#ifdef __cplusplus
extern "C" {
#endif
/* operator delete */
void func_08992F04(void *p);
#ifdef __cplusplus
}

/*
 * SNC deleting destructor of a class whose only work is restoring its own
 * vtable pointer: `if (this) { vptr = vtbl; if (flags & 1) delete this; }`.
 * Written as an in-class helper so the vtable address is hoisted into the
 * first branch delay slot exactly as the compiler-generated destructor does.
 */
struct Agent4Vptr08 {
    int field_00;
    int field_04;
    void *vtable;

    static void destroy(Agent4Vptr08 *self, unsigned int flags, void *vtable) {
        if (self != 0) {
            self->vtable = vtable;
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

struct Agent4Vptr10 {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    void *vtable;

    static void destroy(Agent4Vptr10 *self, unsigned int flags, void *vtable) {
        if (self != 0) {
            self->vtable = vtable;
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};
#endif

#endif
