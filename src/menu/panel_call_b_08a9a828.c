/* Looks up the record for the owner's id at +0x8050 and calls the setter on self + 0x158 with slot 1. */
extern int D_089D712C;
extern int func_088CB164(int base, int id);
extern void func_0891A970(int p, int slot, int id, int b);
void func_08A9A828(int self) {
    int r = func_088CB164(D_089D712C + 0xF58, *(int *)(*(int *)(self + 0x154) + 0x8050));
    char *q = (char *)(r + 0x10);
    func_0891A970(self + 0x158, 1, *(int *)(*(int *)(self + 0x154) + 0x8050), q[0x764]);
}
