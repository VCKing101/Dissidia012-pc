extern "C" {
extern unsigned char D_089EF1A8[];
extern void func_08992F04(void *);
}

struct UnitEB24 {
    char pad_000[0x8];
    void *member_table;

    static void destroy(UnitEB24 *object, int flags)
    {
        if (object) {
            object->member_table = D_089EF1A8;
            if (flags & 1) func_08992F04(object);
        }
    }
};

extern "C" void func_089EEB24(UnitEB24 *object, int flags)
{
    UnitEB24::destroy(object, flags);
}
