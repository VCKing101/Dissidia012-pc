extern "C" {
extern unsigned char D_089EF188[];
extern void func_08992F04(void *);
}

struct UnitEABC {
    char pad_000[0x8];
    void *member_table;

    static void destroy(UnitEABC *object, int flags)
    {
        if (object) {
            object->member_table = D_089EF188;
            if (flags & 1) func_08992F04(object);
        }
    }
};

extern "C" void func_089EEABC(UnitEABC *object, int flags)
{
    UnitEABC::destroy(object, flags);
}
