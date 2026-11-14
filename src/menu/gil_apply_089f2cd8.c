/* Reads an amount (capped at 10000), applies it when non-zero and reports it. */
extern int func_089EFB50(void *self, int kind, int index);
extern void func_089FC7EC(void *self, int amount);
extern void func_089F2BF8(void *self, int amount);

void func_089F2CD8(void *self)
{
    int amount = 0;
    int value = func_089EFB50(self, 0x14, -1);
    if (value != 0) {
        if (value > 10000) {
            value = 10000;
        }
        amount = value;
        func_089FC7EC(self, amount);
    }
    func_089F2BF8(self, amount);
}
