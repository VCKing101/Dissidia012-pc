struct Pools39E5C {
    int block;
    int pool;
};

struct Screen39E5C {
    char pad_00[0x1274];
    Pools39E5C *pools;
};

struct Heap39E5C {
    char pad_000[0x710];
    int handle;
};

extern "C" {
extern int D_089D4960;
extern Heap39E5C *D_089D5EC0;
extern char D_08A3BE38[];
extern void func_08844588(int o);
extern int func_0885C6CC(int handle, int size, int a, int b, int c, int d);
extern int func_0885C40C(int pool, int size, void *name, int flags);
}

extern "C" void func_08A39E5C(Screen39E5C *self)
{
    int size;
    int one;
    func_08844588(D_089D4960);
    size = 0x200000;
    one = 1;
    self->pools->pool = func_0885C6CC(D_089D5EC0->handle, size, 0, 0, one, 0);
    self->pools->block = func_0885C40C(self->pools->pool, size, D_08A3BE38, 0x40);
}
