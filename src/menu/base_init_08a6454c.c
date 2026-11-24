/* Runs two base initialisers (the second takes the sixth argument) and sets the state word to 1. */
extern void func_08A64860(int self, int a, int b, int c, int d, int e);
extern void func_08A649E4(int self, int e);
void func_08A6454C(int self, int a, int b, int c, int d, int e) {
    int s = self;
    int k = e;
    func_08A64860(s, a, b, c, d, k);
    func_08A649E4(s, k);
    *(int *)(s + 0x10) = 1;
}
