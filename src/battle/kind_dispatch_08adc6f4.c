/* Maps an event kind to a class (0, 1, 3 or 2) and forwards it to func_08ADC770 unless unmapped. */
extern void func_08ADC770(int self, int arg, int cls);

void func_08ADC6F4(int self, int kind, int arg)
{
    int cls = -1;
    switch (kind) {
    case 0:
        cls = 0;
        break;
    case 1:
        cls = 1;
        break;
    case 0x1A:
    case 0x29:
    case 0x2A:
        cls = 3;
        break;
    case 0x2C:
        cls = 2;
    }
    if (cls != -1) {
        func_08ADC770(self, arg, cls);
    }
}
