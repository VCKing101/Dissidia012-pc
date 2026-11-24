// Counts the units (func_08841F64 iteration) with type byte 0 whose probed result byte matches *(int *)(self+4) or that value minus one.
extern "C" {
extern void *func_08899300(void);
extern void *func_08841F64(void *);
extern void func_089A3868(void *, void *);
extern void func_088976E4(void *, void *, void *, int, int, int, int);
}
struct Unit57 {
    char pad0[0x100];
    char pos[0x442];
    unsigned char type;
    static bool isZero(Unit57 *u) { return u->type == 0; }
};
struct Self57 {
    int pad0;
    int value;
};
extern "C" int func_08B60238(Self57 *self)
{
    void *world = func_08899300();
    int count = 0;
    Unit57 *u = 0;
    unsigned char out[0x30];
    char pos[0x10];
    while ((u = (Unit57 *)func_08841F64(u)) != 0) {
        if (Unit57::isZero(u)) {
            func_089A3868(pos, u->pos - 0 + 0);
            func_088976E4(world, out, pos, 2, 1, 0, 0);
            {
                unsigned char r;
                int v = self->value;
                r = out[0x2C];
                if (v == r || v + 1 == r) {
                    count++;
                }
            }
        }
    }
    return count;
}
