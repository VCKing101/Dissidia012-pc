/* Optionally run the pre-step, then draw three halves, or the single variant when the flag byte is set. */
extern void func_08986D38(int self, int a);
extern void func_08986CC8(int self, int a, int b, int c, int d);
extern void func_08986C38(int self, int a);
void func_08986B00(int self, int a)
{
    int h;
    if (*(unsigned char *)(self + 0xB3) != 0) {
        func_08986D38(self, a);
    }
    if (*(unsigned char *)(self + 0xC0) != 0) {
        func_08986C38(self, a);
    } else {
        h = *(int *)(self + 0x20) / 2;
        func_08986CC8(self, a, *(int *)(self + 0x1C), *(int *)(self + 0x44), h);
        func_08986CC8(self, a, *(int *)(self + 0x1C), *(int *)(self + 0x40), h);
        func_08986CC8(self, a, *(int *)(self + 0x1C), *(int *)(self + 0x48), h);
    }
}
