/* Returns 0 when the owner's field at +0x802C is null, otherwise the routine's result. */
extern int func_08AB2EA4(void);
int func_08A9B0DC(int self) {
    if (*(int *)(*(int *)(self + 0x154) + 0x802C) != 0) {
        return func_08AB2EA4();
    } else {
        return 0;
    }
}
