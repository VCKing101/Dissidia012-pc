/* If bit 0x10000 of self+0x44 is set: func_088FE9CC, clear it, func_08B35CF0(self, 0), func_088AC8C8(D_089D712C, 1), state 5 at +0xAC; else calls func_08B35680(self). */
struct Self { char pad[0x44]; unsigned flags; char pad2[0xAC - 0x48]; int state; };
extern "C" void *D_089D712C;
extern "C" void func_088FE9CC(void);
extern "C" void func_08B35CF0(Self *self, int z);
extern "C" void func_088AC8C8(void *p, int one);
extern "C" void func_08B35680(Self *self);
struct Kept { Self *v; Kept(Self *s) { v = s; } };
struct T { static bool has(Self *s) { return (s->flags & 0x10000) != 0; } };
extern "C" void func_08B35988(Self *self)
{
    if (T::has(self)) {
        Kept k(self);
        func_088FE9CC();
        self = k.v;
        self->flags = self->flags & ~0x10000;
        func_08B35CF0(self, 0);
        func_088AC8C8(D_089D712C, 1);
        self = k.v;
        self->state = 5;
    } else {
        func_08B35680(self);
    }
}
