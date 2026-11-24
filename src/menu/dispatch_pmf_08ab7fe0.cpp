// Calls the handler (pointer to member function) selected from a constant table by the owner's state index, then the global completion callback if set.
struct Menu {
    char d0[0x138];
    int *owner;
};
typedef void (Menu::*Handler)();
extern "C" {
void *func_08935524(void *d, const void *s, int n);
extern const char D_08ABB9E8[];
extern struct Blk { char g[0xC]; void (*cb)(int); int arg; } D_09B6D818;
}
extern "C" void func_08AB7FE0(Menu *self) {
    Handler table[5];
    func_08935524(table, D_08ABB9E8, 0x28);
    (self->*table[*(int *)((char *)self->owner + 0xD2C)])();
    if (D_09B6D818.cb != 0) {
        D_09B6D818.cb(D_09B6D818.arg);
    }
}
