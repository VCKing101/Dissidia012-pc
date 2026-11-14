extern "C" {
extern unsigned char *D_089D7280;
extern int D_089D7260;
extern int func_088AF0EC(int v);
}
struct Net08A1F5C4 {
    static int Check() {
        int r = -1;
        if (func_088AF0EC(D_089D7260) == 0) {
            r = 1;
        }
        return r;
    }
};
extern "C" int func_08A1F5C4(void)
{
    int result = 0;
    if (*D_089D7280 == 0) {
        result = Net08A1F5C4::Check();
    }
    return result;
}
