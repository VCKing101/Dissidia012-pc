// Dispatches on kind (2,3,4,10,11,0x10,0x15,0x1D) to the matching handler(s) with the remaining arguments.
extern void func_08AF03DC(void *, int, int);
extern void func_08AF087C(void *, int, int);
extern void func_08AF061C(void *, int, int);
extern void func_08AF07EC(void *, int, int);
extern void func_08AF0838(void *, int, int);
extern void func_08AF03E8(void *, int, int);
void func_08AF02C0(void *self, int kind, int a, int b)
{
    switch (kind) {
    case 2:
        func_08AF03DC(self, a, b);
        break;
    case 3:
        func_08AF03DC(self, a, b);
        break;
    case 0x1D:
        func_08AF03E8(self, a, b);
        func_08AF03DC(self, a, b);
        break;
    case 10:
    case 11:
        func_08AF061C(self, a, b);
        func_08AF03DC(self, a, b);
        break;
    case 0x10:
        func_08AF07EC(self, a, b);
        break;
    case 0x15:
        func_08AF0838(self, a, b);
        break;
    case 4:
        func_08AF087C(self, a, b);
    }
}
