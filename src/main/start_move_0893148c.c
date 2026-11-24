/* When there are entries: finish a pending state 3, record the settings, restart the cursor and enter state 2. */
extern void func_08931908(int self);
extern void func_088D46E4(int p, int a, int b, int c, int d, int e);
extern void func_089306E4(int p);
void func_0893148C(int self, int a, int b, unsigned char c, unsigned char d)
{
    if (!(0 < *(int *)(self + 0x260))) {
        return;
    }
    if (*(int *)(self + 0x28) == 3) {
        func_08931908(self);
        *(int *)(self + 0x28) = 1;
    }
    *(int *)(self + 0x264) = a;
    *(unsigned char *)(self + 0x298) = c;
    *(unsigned char *)(self + 0x299) = d;
    func_088D46E4(self + 0x268, b, 0, *(int *)(self + 0x260) - 1, 0, -1);
    func_089306E4(self + *(int *)(self + 0x26C) * 0x5C + 0x38);
    *(int *)(self + 0x294) = 0;
    *(int *)(self + 0x28) = 2;
}
