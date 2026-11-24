// Set up the static session record, register it globally and drop it again when start fails.
extern "C" char D_09B69B50[];
extern "C" void *D_089D8360;
extern "C" void func_08902C28(void *p);
extern "C" int func_08902C50(void *p, int arg);
extern "C" bool func_0890E7D8(int arg)
{
    void *r = 0;
    void *p = D_09B69B50;
    if (p) {
        func_08902C28(p);
        r = p;
    }
    D_089D8360 = r;
    int res = func_08902C50(r, arg);
    if (res == 1) {
        return true;
    }
    D_089D8360 = 0;
    return false;
}
