struct Screen435D0 {
    char pad_00[0x134];
    int block;
    int pool;
};

extern "C" {
extern char D_08A4566C[];
extern int func_0888204C(int size);
extern int func_0885C40C(int pool, int size, void *name, int flags);
}

extern "C" void func_08A435D0(Screen435D0 *self)
{
    int size = 0x270800;
    self->pool = func_0888204C(size);
    self->block = func_0885C40C(self->pool, size, D_08A4566C, 0x40);
}
