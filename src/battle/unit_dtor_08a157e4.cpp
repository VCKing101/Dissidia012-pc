extern "C" {
extern unsigned char D_08A158B8[];
extern void func_08A15834(void *);
}

struct Unit57E4 {
    void *member_table;

    static void destroy(Unit57E4 *object, int flags)
    {
        if (object) {
            object->member_table = D_08A158B8;
            if (flags & 1) func_08A15834(object);
        }
    }
};

extern "C" void func_08A157E4(Unit57E4 *object, int flags)
{
    Unit57E4::destroy(object, flags);
}
