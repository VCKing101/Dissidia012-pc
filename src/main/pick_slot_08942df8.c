/* Finds a slot with func_08942244(self); if found, passes the slot's entry and the word at 0x28 to func_089428F8; returns the slot or -1. */
extern int func_08942244(int *);
extern void func_089428F8(int, int);
int func_08942DF8(int *self) {
    int slot = func_08942244(self);
    if (slot == -1) { return -1; }
    func_089428F8(((int *)self[0])[slot], self[10]);
    return slot;
}
