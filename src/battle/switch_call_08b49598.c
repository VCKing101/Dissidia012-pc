// Dispatches on a1: 0 calls func_08B4962C(a0), 1 calls func_08B495DC(a0).
extern void func_08B4962C(void *);
extern void func_08B495DC(void *);
void func_08B49598(void *self, int mode)
{
    switch (mode) {
    case 0:
        func_08B4962C(self);
        break;
    case 1:
        func_08B495DC(self);
        break;
    }
}
