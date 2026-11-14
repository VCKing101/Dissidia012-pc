extern int D_089D7F98;
extern int D_089D7FC8[];
extern int D_089D8018[];
extern void func_088FE9AC(void);
extern void func_088FE6A0(int *ids);

void func_088FEAEC(unsigned char close)
{
    if (D_089D7F98 == 2) {
        return;
    }
    if (close) {
        func_088FE9AC();
    }
    D_089D7F98 = 2;
    func_088FE6A0(D_089D7FC8);
    func_088FE6A0(D_089D8018);
}
