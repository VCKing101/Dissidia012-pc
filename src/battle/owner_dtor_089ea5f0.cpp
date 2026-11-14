extern "C" {
extern unsigned char D_089EB788[];
extern void func_08992F04(void *);
extern void func_0899372C(void *, unsigned int, unsigned int, void *, int, int);
extern void func_089EA74C(void *);
extern void func_089EB898(void *, int);
extern void func_089EBBF0(void *);
}

struct PoolA5F0 {
    char pad_000[0x80];
    char entries[0x10][0x78];

    static void destroy(PoolA5F0 *pool)
    {
        if (pool) {
            func_0899372C(pool->entries, 0x10, 0x78, (void *)func_089EBBF0, 0, 0);
        }
    }
};

struct OwnerA5F0 {
    char pad_000[0x8];
    PoolA5F0 pool;
    char pad_808[0x81C - 0x808];
    char field_81C[0x2C];
    void *member_table;

    static void destroy(OwnerA5F0 *object, int flags)
    {
        if (object) {
            object->member_table = D_089EB788;
            func_089EA74C(object);
            func_089EB898(object->field_81C, 2);
            if ((PoolA5F0 *)((char *)object + 8)) {
                func_0899372C(object->pool.entries, 0x10, 0x78, (void *)func_089EBBF0, 0, 0);
            }
            if (flags & 1) func_08992F04(object);
        }
    }
};

extern "C" void func_089EA5F0(OwnerA5F0 *object, int flags)
{
    OwnerA5F0::destroy(object, flags);
}
