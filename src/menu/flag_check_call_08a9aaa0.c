/* Returns 0 when both flag bits (0x40 and 0x1000) of the owner's word are set, otherwise the routine's result. */
extern int func_08B2BA5C(int v);
int func_08A9AAA0(int self) {
    unsigned f = *(unsigned *)(*(int *)(self + 0x154) + 0x81B4);
    if ((f & 0x40) != 0) {
        if ((f & 0x1000) != 0) {
            return 0;
        }
    }
    return func_08B2BA5C(*(int *)(*(int *)(self + 0x154) + 0x9B0));
}
