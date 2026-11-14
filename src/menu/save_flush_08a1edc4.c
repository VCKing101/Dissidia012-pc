extern int D_089D712C;
extern int func_08A1D8E4(void);
extern void func_088F7D0C(int a);
extern void func_088F5508(int a);

void func_08A1EDC4(int self)
{
    if (func_08A1D8E4() != 0) {
        func_088F7D0C(*(int *)(*(int *)(self + 0x154) + 0x6964));
    } else {
        func_088F5508(D_089D712C + 0x1FECC);
    }
}
