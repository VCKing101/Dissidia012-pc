struct Work4AA84 {
    char pad_000[0xBE4];
    int handle;
    int channel;
};

struct Screen4AA84 {
    char pad_00[0x138];
    int pool;
    char pad_13C[0x14C - 0x13C];
    Work4AA84 *work;
};

extern "C" {
extern int D_089D6998;
extern int func_0885C6CC(int pool, int size, int a, int b, int c, int d);
extern int func_0887E74C(int o, int handle, int size);
}

extern "C" void func_08A4AA84(Screen4AA84 *self)
{
    int size = 0x20000;
    int one = 1;
    self->work->handle = func_0885C6CC(self->pool, size, 0, 0, one, 0);
    self->work->channel = func_0887E74C(D_089D6998, self->work->handle, size);
}
