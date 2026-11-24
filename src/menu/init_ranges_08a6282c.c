/* Initialises four range controls of the panel (the second clamped to the table maximum and flagged) with fixed limits. */
extern int D_08A636D8[];
extern void func_088D46E4(int p, int cur, int a, int max, int b, int c);
void func_08A6282C(int self) {
    func_088D46E4(self + 0xD30, 0, 0, 0xB, 0, -1);
    func_088D46E4(self + 0xD5C, 0, 0, D_08A636D8[*(int *)(self + 0xD34)] - 1, 0, -1);
    *(unsigned *)(self + 0xD5C) = *(unsigned *)(self + 0xD5C) | 1;
    func_088D46E4(self + 0xD8C, 0, 0, 6, 0, -1);
    func_088D46E4(self + 0xDB8, 0, 0, 4, 0, 1);
}
