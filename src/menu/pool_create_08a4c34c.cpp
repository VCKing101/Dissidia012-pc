struct Screen4C34C {
    char pad_00[0x38];
    int pool;
    int block;
};

extern "C" {
extern char D_08A4FF38[];
extern int func_0888204C(int size);
extern int func_0885C40C(int pool, int size, void *name, int flags);
}

extern "C" void func_08A4C34C(Screen4C34C *self)
{
    int size = 0x270800;
    self->pool = func_0888204C(size);
    self->block = func_0885C40C(self->pool, size, D_08A4FF38, 0x40);
}
