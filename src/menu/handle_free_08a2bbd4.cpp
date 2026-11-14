struct Screen2BBD4 {
    char pad_00[0x134];
    int handle;
};

struct Pool2BBD4 {
    int pad_00;
    int pool;
};

template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

extern "C" {
extern Pool2BBD4 D_09B6DA10;
extern char D_08A2E7F0[];
extern int func_0885C40C(int pool, int size, void *name, int flags);
}

extern "C" void func_08A2BBD4(Screen2BBD4 *self)
{
    int pool = D_09B6DA10.pool;
    if (pool != 0) {
        Kept<Screen2BBD4 *> saved(self);
        int handle = func_0885C40C(pool, 0xBA800, D_08A2E7F0, 0x40);
        self = saved.value;
        self->handle = handle;
    } else {
        self->handle = 0;
    }
}
