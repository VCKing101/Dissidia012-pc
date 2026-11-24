// Dispatches on kind: 0x16 runs func_08AEC628, 5 runs func_08AEC6FC, 2 runs func_08AEC7D8 then func_08AECA90, 3 runs func_08AECA90.
extern void func_08AEC7D8(void *, int, int);
extern void func_08AECA90(void *, int, int);
extern void func_08AEC6FC(void *, int, int);
extern void func_08AEC628(void *, int, int);
void func_08AEC558(void *self, int kind, int a, int b)
{
    switch (kind) {
    case 0x16:
        func_08AEC628(self, a, b);
        break;
    case 5:
        func_08AEC6FC(self, a, b);
        break;
    case 2:
        func_08AEC7D8(self, a, b);
        func_08AECA90(self, a, b);
        break;
    case 3:
        func_08AECA90(self, a, b);
        break;
    }
}
