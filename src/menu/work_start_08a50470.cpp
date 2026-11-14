struct Work50470 {
    char pad_00[0x6B18];
    int handle;
};

struct Screen50470 {
    char pad_00[0x10];
    int pool;
    char pad_14[0x58 - 0x14];
    Work50470 *work;
};

extern "C" {
extern int D_089D6998;
extern int func_0885C6CC(int pool, int size, int a, int b, int c, int d);
extern int func_0887E630(int o, int handle, int size);
}

extern "C" void func_08A50470(Screen50470 *self)
{
    int size = 0x40000;
    int one = 1;
    self->work->handle = func_0885C6CC(self->pool, size, 0, 0, one, 0);
    func_0887E630(D_089D6998, self->work->handle, size);
}
