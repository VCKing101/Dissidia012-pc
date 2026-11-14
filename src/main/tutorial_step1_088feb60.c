extern int D_089D7F98;
extern int D_089D8088[];
extern void func_088FE9AC(void);
extern void func_088FE6A0(int *ids);

void func_088FEB60(unsigned char close)
{
    if (D_089D7F98 == 1) {
        return;
    }
    if (close) {
        func_088FE9AC();
    }
    D_089D7F98 = 1;
    func_088FE6A0(D_089D8088);
}
