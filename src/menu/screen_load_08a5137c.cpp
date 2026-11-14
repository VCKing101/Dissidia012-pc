struct Work5137C {
    char pad_0000[4];
    int step;
    char pad_0008[0x79A4 - 8];
    int mode;
    int source;
    char pad_79AC[0x7BC0 - 0x79AC];
    char group_7BC0[0x778];
    int handle_8338;
};

struct Screen5137C {
    char pad_00[0x10];
    int pool;
    char task[0x44];
    Work5137C *work;
};

extern "C" {
extern void *D_089D7830;
extern int D_08B01CF8;
extern char D_08A51C68[];
extern void func_08865190(void *task);
extern void func_0885C538(int pool, int source, int flag);
extern void func_088D0FD4(void *loader, int pool, int size, char kind, void *name);
extern void func_08A50470(Screen5137C *self);
extern void func_08AF33E0(int o, void *group);
extern void func_088BA0AC(int handle, void *group);
}

/* Inlined call on the loader in D_089D7830. */
struct Loader5137C {
    static void Load(int pool, int size, char kind, void *name) {
        func_088D0FD4(D_089D7830, pool, size, kind, name);
    }
};

extern "C" bool func_08A5137C(Screen5137C *self)
{
    func_08865190((char *)self + 0x14);
    func_0885C538(self->pool, self->work->source, 0x40);
    Loader5137C::Load(self->pool, 0x100000, 0x1A, D_08A51C68);
    func_08A50470(self);
    if (self->work->mode == 1) {
        func_08AF33E0(D_08B01CF8, self->work->group_7BC0);
        func_088BA0AC(self->work->handle_8338, self->work->group_7BC0);
    }
    self->work->step = 0xE;
    return true;
}
