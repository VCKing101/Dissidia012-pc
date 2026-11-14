extern "C" {
extern int func_08A408E8(int size);
extern void func_08A3E0F8(int obj);
}

struct Create08A3E334 {
    static int Make() {
        int result = 0;
        int obj = func_08A408E8(0x2a18);
        if (obj != 0) {
            func_08A3E0F8(obj);
            result = obj;
        }
        return result;
    }
};

extern "C" int func_08A3E334(void)
{
    return Create08A3E334::Make();
}
