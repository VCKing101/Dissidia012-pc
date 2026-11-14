extern "C" {
extern int func_08A408E8(int size);
extern void func_08A37890(int obj);
}

struct Create08A37A7C {
    static int Make() {
        int result = 0;
        int obj = func_08A408E8(0xf68);
        if (obj != 0) {
            func_08A37890(obj);
            result = obj;
        }
        return result;
    }
};

extern "C" int func_08A37A7C(void)
{
    return Create08A37A7C::Make();
}
