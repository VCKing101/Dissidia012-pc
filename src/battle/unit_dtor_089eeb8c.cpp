extern "C" {
extern unsigned char D_089EF1C8[];
extern void func_08992F04(void *);
}

struct UnitEB8C {
    char pad_000[0x8];
    void *member_table;

    static void destroy(UnitEB8C *object, int flags)
    {
        if (object) {
            object->member_table = D_089EF1C8;
            if (flags & 1) func_08992F04(object);
        }
    }
};

extern "C" void func_089EEB8C(UnitEB8C *object, int flags)
{
    UnitEB8C::destroy(object, flags);
}
