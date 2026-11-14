extern unsigned char D_089D69DC;
extern unsigned char D_089D69DD;
extern void *D_089D69D4;
extern void func_08882F68(void *session, int reason);
extern void func_088837EC(void);
extern void func_08883AB8(void);

void func_0888337C(unsigned char force)
{
    if (D_089D69DC == 0) {
        return;
    }
    if (!force) {
        /* a check whose body the build compiled out */
        (void)force;
    }
    if (D_089D69D4 != 0) {
        func_08882F68(D_089D69D4, 3);
        D_089D69D4 = 0;
    }
    func_088837EC();
    func_08883AB8();
    D_089D69DC = 0;
    D_089D69DD = 0;
}
