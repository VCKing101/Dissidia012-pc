extern int D_089D7F98;
extern int D_089D80D0[];
extern void func_088FE9AC(void);
extern void func_088FE6A0(int *ids);

void func_088FEC30(unsigned char close)
{
    if (D_089D7F98 == 3) {
        return;
    }
    if (close) {
        func_088FE9AC();
    }
    D_089D7F98 = 3;
    func_088FE6A0(D_089D80D0);
}
