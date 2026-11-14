extern "C" {
extern unsigned char D_089E8F58[];
extern void func_08992F04(void *);
}

struct Unit8EA8 {
    char pad_000[0x4];
    void *member_table;

    static void destroy(Unit8EA8 *object, int flags)
    {
        if (object) {
            object->member_table = D_089E8F58;
            if (flags & 1) func_08992F04(object);
        }
    }
};

extern "C" void func_089E8EA8(Unit8EA8 *object, int flags)
{
    Unit8EA8::destroy(object, flags);
}
