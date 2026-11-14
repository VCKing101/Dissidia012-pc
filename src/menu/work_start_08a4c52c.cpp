struct Work4C52C {
    char pad_00[8];
    int handle;
};

struct Screen4C52C {
    char pad_00[0x3C];
    int pool;
    char pad_40[0x5C - 0x40];
    Work4C52C *work;
};

extern "C" {
extern int D_089D6998;
extern int func_0885C6CC(int pool, int size, int a, int b, int c, int d);
extern int func_0887E630(int o, int handle, int size);
}

extern "C" void func_08A4C52C(Screen4C52C *self)
{
    int size = 0x40000;
    int one = 1;
    self->work->handle = func_0885C6CC(self->pool, size, 0, 0, one, 0);
    func_0887E630(D_089D6998, self->work->handle, size);
}
