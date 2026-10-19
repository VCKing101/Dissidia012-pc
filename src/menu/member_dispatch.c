#include "menu/execution_state.h"

typedef struct MenuMemberCall {
    signed short adjustment;
    signed short flags;
    unsigned int target;
} MenuMemberCall;

extern const MenuMemberCall D_089E09E8[20];
extern unsigned char D_089D5F20;
extern void *func_08935524(void *destination, const void *source, unsigned int size);
extern void func_08862450(void *object);

void func_089DD5DC(MenuRuntimePrefix *self) {
    MenuMemberCall table[20];
    MenuMemberCall *member;
    unsigned char *adjusted;
    void (*target)(MenuRuntimePrefix *);
    int flags;
    func_08935524(table, D_089E09E8, sizeof(table));
    if (!D_089D5F20) func_08862450(self->unknown_01E0);
    member = &table[self->field_01DC->field_0008];
    adjusted = (unsigned char *)self + member->adjustment;
    flags = member->flags;
    if (flags) {
        member = (MenuMemberCall *)*(void **)(adjusted + member->target) + flags;
        adjusted += member->adjustment;
        target = (void (*)(MenuRuntimePrefix *))member->target;
    } else {
        target = (void (*)(MenuRuntimePrefix *))member->target;
    }
    target((MenuRuntimePrefix *)adjusted);
}
