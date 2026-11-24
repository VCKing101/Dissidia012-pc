/* Optionally run the pre-step, then draw with one of two variants chosen by the flag byte. */
extern void func_08986D38(int self, int a);
extern void func_08986D00(int self, int a, int b, int c, int d);
extern void func_08986CC8(int self, int a, int b, int c, int d);
void func_089869D4(int self, int a)
{
    if (*(unsigned char *)(self + 0xB3) != 0) {
        func_08986D38(self, a);
    }
    if (*(unsigned char *)(self + 0xC0) != 0) {
        func_08986D00(self, a, *(int *)(self + 0x1C), *(int *)(self + 0x40), *(int *)(self + 0x20));
    } else {
        func_08986CC8(self, a, *(int *)(self + 0x1C), *(int *)(self + 0x40), *(int *)(self + 0x20));
    }
}
