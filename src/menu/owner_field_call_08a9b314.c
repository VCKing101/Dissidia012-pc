/* Returns the routine's result on the owner's field at +0x9C0 when the check passes, otherwise 0. */
extern int func_08A9B2D4(int p);
extern int func_08B2BA5C(int v);
int func_08A9B314(int self) {
    int r = func_08A9B2D4(self);
    if (r != 0) {
        return 0;
    } else {
        return func_08B2BA5C(*(int *)(*(int *)(self + 0x154) + 0x9C0));
    }
}
