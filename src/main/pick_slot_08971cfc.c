/* Finds a slot with func_08941FC0(self); if found, passes the slot's entry and the word at 0x28 to func_08971554; returns the slot or -1. */
extern int func_08941FC0(int *);
extern void func_08971554(int, int);
int func_08971CFC(int *self) {
    int slot = func_08941FC0(self);
    if (slot == -1) { return -1; }
    func_08971554(((int *)self[0])[slot], self[10]);
    return slot;
}
