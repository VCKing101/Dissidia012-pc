// Calls the handler (pointer to member function) selected from a constant table by the owner's state index, then the global completion callback if set.
struct Menu {
    char d0[0x14C];
    int *owner;
};
typedef void (Menu::*Handler)();
extern "C" {
void *func_08935524(void *d, const void *s, int n);
extern const char D_08AC4564[];
extern struct Blk { char g[0x14]; void (*cb)(int); int arg; } D_09B6D9D8;
}
extern "C" void func_08ABC0B8(Menu *self) {
    Handler table[36];
    func_08935524(table, D_08AC4564, 0x120);
    (self->*table[*(int *)((char *)self->owner + 0xA5C)])();
    if (D_09B6D9D8.cb != 0) {
        D_09B6D9D8.cb(D_09B6D9D8.arg);
    }
}
