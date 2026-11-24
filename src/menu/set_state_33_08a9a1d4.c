/* Calls the manager routine on the global, then sets the owner's state word to 0x33. */
extern int D_089D69E8;
extern void func_088853CC(int a, int b);
void func_08A9A1D4(int self) {
    int g = D_089D69E8;
    int zero = 0;
    func_088853CC(g, zero);
    *(int *)(*(int *)(self + 0x154) + 0x910) = 0x33;
}
