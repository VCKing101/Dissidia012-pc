/* Lays out the controls: the long set when the current mode's value is 2, otherwise the short set. */
extern void func_08A8D480(int self, int a, int b);
extern void func_08A8DC60(int self, int a, int b);
void func_08A8C540(int self) {
    if (*(int *)(*(int *)(self + 8) + *(int *)(*(int *)(self + 8) + 0x2B4) * 4 + 0x308) != 2) {
        func_08A8D480(self, 0x13, -1);
        func_08A8D480(self, 0x10, 0x38);
        func_08A8DC60(self, 10, 0x41);
    } else {
        func_08A8D480(self, 0x13, -1);
        func_08A8D480(self, 0x10, 0x38);
        func_08A8D480(self, 4, 0x2B);
        func_08A8D480(self, 5, -1);
        func_08A8D480(self, 6, -1);
        func_08A8D480(self, 7, -1);
        func_08A8D480(self, 8, -1);
        func_08A8D480(self, 9, -1);
        func_08A8DC60(self, 0, 0x41);
    }
}
