extern "C" {
extern unsigned char D_089F13D8[];
extern void func_089F1354(void *);
}

struct Unit1304 {
    void *member_table;

    static void destroy(Unit1304 *object, int flags)
    {
        if (object) {
            object->member_table = D_089F13D8;
            if (flags & 1) func_089F1354(object);
        }
    }
};

extern "C" void func_089F1304(Unit1304 *object, int flags)
{
    Unit1304::destroy(object, flags);
}
