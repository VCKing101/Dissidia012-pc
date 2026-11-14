extern void func_08AECEE4(void *self, int a, int b);
extern void func_08AECEDC(void *self, int a, int b);

void func_08AECE30(void *self, int kind, int a, int b)
{
    switch (kind) {
    case 0x26:
        func_08AECEDC(self, a, b);
        break;
    case 4:
        func_08AECEE4(self, a, b);
        break;
    }
}
