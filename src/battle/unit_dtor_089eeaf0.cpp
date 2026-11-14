extern "C" {
extern unsigned char D_089EF198[];
extern void func_08992F04(void *);
}

struct UnitEAF0 {
    char pad_000[0x8];
    void *member_table;

    static void destroy(UnitEAF0 *object, int flags)
    {
        if (object) {
            object->member_table = D_089EF198;
            if (flags & 1) func_08992F04(object);
        }
    }
};

extern "C" void func_089EEAF0(UnitEAF0 *object, int flags)
{
    UnitEAF0::destroy(object, flags);
}
