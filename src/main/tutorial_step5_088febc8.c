extern int D_089D7F98;
extern int D_089D80B8[];
extern void func_088FE9AC(void);
extern void func_088FE6A0(int *ids);

void func_088FEBC8(unsigned char close)
{
    if (D_089D7F98 == 5) {
        return;
    }
    if (close) {
        func_088FE9AC();
    }
    D_089D7F98 = 5;
    func_088FE6A0(D_089D80B8);
}
