#include "menu/runtime_prefix.h"

extern struct MainCallbackBlock *func_08864FF0(struct MainCallbackBlock *self,
    unsigned int a, unsigned int b, signed char c);

extern void *func_0889F63C(void *object);
extern void func_0889F674(void *object, unsigned int flags);
extern void func_08992F04(void *object);
extern void func_08866AA8(unsigned int a, unsigned int b, unsigned int c);
extern unsigned char D_089C8E70[];
extern unsigned char D_089C8E88[];
extern unsigned char D_089C8EA0[];

/* Caller-supplied storage; four subobjects and three observed table pointers.
 * Semantic field/class names remain unknown. No allocation is performed here. */
MenuRuntimePrefix *func_089DD4D0(MenuRuntimePrefix *self) {
    func_08864FF0(&self->callback_0100, 1, 50000, 0x54);
    func_0889F63C(&self->subobjects_014C[0].prefix);
    func_0889F63C(&self->subobjects_014C[1].prefix);
    func_0889F63C(&self->subobjects_014C[2].prefix);
    func_0889F63C(&self->subobjects_014C[3].prefix);
    self->table_01F8 = D_089C8E70;
    self->table_0214 = D_089C8E88;
    self->table_0230 = D_089C8EA0;
    self->field_0134 = 0;
    self->field_0138 = 0;
    self->field_01DC = 0;
    func_08866AA8(0, 0, 0);
    return self;
}

/* Null-guarded reverse subobject teardown, with optional owner release. */
void func_089DD568(MenuRuntimePrefix *self, unsigned int flags) {
    if (self) {
        func_0889F674(&self->subobjects_014C[3].prefix, 2);
        func_0889F674(&self->subobjects_014C[2].prefix, 2);
        func_0889F674(&self->subobjects_014C[1].prefix, 2);
        func_0889F674(&self->subobjects_014C[0].prefix, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
