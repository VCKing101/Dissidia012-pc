extern "C" {
extern unsigned char D_089EF1B8[];
extern void func_08992F04(void *);
}

struct UnitEB58 {
    char pad_000[0x8];
    void *member_table;

    static void destroy(UnitEB58 *object, int flags)
    {
        if (object) {
            object->member_table = D_089EF1B8;
            if (flags & 1) func_08992F04(object);
        }
    }
};

extern "C" void func_089EEB58(UnitEB58 *object, int flags)
{
    UnitEB58::destroy(object, flags);
}
