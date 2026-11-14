struct Work4E5B8 {
    char pad_0000[4];
    int step;
    char pad_0008[0x6EF8 - 8];
    int source;
};

struct Screen4E5B8 {
    char pad_00[4];
    char task[0x38];
    int pool;
    char pad_40[0x5C - 0x40];
    Work4E5B8 *work;
};

extern "C" {
extern void *D_089D7830;
extern char D_08A4FFA0[];
extern void func_08865190(void *task);
extern void func_0885C538(int pool, int source, int flag);
extern void func_088D0FD4(void *loader, int pool, int size, char kind, void *name);
extern void func_08A4C52C(Screen4E5B8 *self);
extern void func_08A4ED34(Screen4E5B8 *self, int a, int b);
}

/* Inlined call on the loader in D_089D7830. */
struct Loader4E5B8 {
    static void Load(int pool, int size, char kind, void *name) {
        func_088D0FD4(D_089D7830, pool, size, kind, name);
    }
};

extern "C" int func_08A4E5B8(Screen4E5B8 *self)
{
    func_08865190(self->task);
    func_0885C538(self->pool, self->work->source, 0x40);
    Loader4E5B8::Load(self->pool, 0x100000, 0x1A, D_08A4FFA0);
    func_08A4C52C(self);
    func_08A4ED34(self, -1, 0);
    self->work->step = 0x11;
    return 1;
}
