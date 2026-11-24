/* Clamps the current value to the table maximum for the mode, calls the setter on self + 0xD5C, sets its low flag bit and runs the follow-up. */
extern int D_08A636D8[];
extern void func_088D46E4(int p, int cur, int a, int max, int b, int c);
extern void func_08A62A3C(int self);
void func_08A629BC(int self) {
    int max = D_08A636D8[*(int *)(self + 0xD34)] - 1;
    int cur = *(int *)(self + 0xD60);
    if (max < cur) {
        cur = max;
    }
    func_088D46E4(self + 0xD5C, cur, 0, max, 0, -1);
    *(unsigned *)(self + 0xD5C) = *(unsigned *)(self + 0xD5C) | 1;
    func_08A62A3C(self);
}
