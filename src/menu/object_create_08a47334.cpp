extern "C" {
extern int func_08A408E8(int size);
extern void func_08A4718C(int obj);
}

struct Create08A47334 {
    static int Make() {
        int result = 0;
        int obj = func_08A408E8(0x41c);
        if (obj != 0) {
            func_08A4718C(obj);
            result = obj;
        }
        return result;
    }
};

extern "C" int func_08A47334(void)
{
    return Create08A47334::Make();
}
