extern int D_08A0B6A0;
extern int func_088F29D0(void);
extern void func_088F29F4(void);
extern void func_089EF05C(int handle, int mode, int value);

void func_089FE618(void) {
    if (func_088F29D0() != 0) {
        func_089EF05C(D_08A0B6A0, 1, -1);
    } else {
        func_089EF05C(D_08A0B6A0, 2, -1);
        func_088F29F4();
    }
}
