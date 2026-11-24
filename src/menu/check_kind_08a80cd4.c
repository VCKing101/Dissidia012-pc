/* Returns 1 when the lookup returns 0x17 and the owner's field at +0x61C is not -1. */
extern int func_08A81188(int self);
int func_08A80CD4(int self) {
    int r = func_08A81188(self);
    if (r == 0x17 && *(int *)(*(int *)(self + 0x28) + 0x61C) != -1) {
        return 1;
    } else {
        return 0;
    }
}
