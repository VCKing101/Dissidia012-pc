extern "C" {
extern unsigned char D_089EB850[];
extern void func_08992F04(void *);
}

struct UnitB7D0 {
    char pad_000[0x2C];
    void *member_table;

    static void destroy(UnitB7D0 *object, int flags)
    {
        if (object) {
            object->member_table = D_089EB850;
            if (flags & 1) func_08992F04(object);
        }
    }
};

extern "C" void func_089EB7D0(UnitB7D0 *object, int flags)
{
    UnitB7D0::destroy(object, flags);
}
