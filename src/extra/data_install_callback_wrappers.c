#include "extra/data_install.h"

/* The wrappers load this context word and forward the PSP callback arguments.
 * The context's concrete class/type has not yet been established. */
extern u32 D_089DE79C;
extern u32 func_089DD5F8(u32 context, u32 count, u32 flags, void *userdata);
extern u32 func_089DD62C(u32 context, u32 count, u32 event, void *userdata);
extern void func_089DDE10(u32 context, u32 value, void *userdata);
extern void func_089DDE20(u32 context, u32 value, void *userdata);

u32 func_089DE69C(u32 count, u32 flags, void *userdata) {
    return func_089DD5F8(D_089DE79C, count, flags, userdata);
}

u32 func_089DE6C8(u32 count, u32 event, void *userdata) {
    return func_089DD62C(D_089DE79C, count, event, userdata);
}

void func_089DE6F4(u32 value, void *userdata) {
    func_089DDE10(D_089DE79C, value, userdata);
}

void func_089DE71C(u32 value, void *userdata) {
    func_089DDE20(D_089DE79C, value, userdata);
}
