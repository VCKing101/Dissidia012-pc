/* Computes max index from a table by mode, clamps the current value to it and calls the setter on self + 0x5E8. */
extern int D_08A5DDC8[];
extern void func_088D46E4(int p, int cur, int a, int max, int b, int c);
void func_08A5D5F4(int self) {
    int cur = *(int *)(self + 0x5EC);
    int max = D_08A5DDC8[*(int *)(self + 0x618)] - 1;
    if (max < cur) {
        cur = max;
    }
    func_088D46E4(self + 0x5E8, cur, 0, max, 0, -1);
}
