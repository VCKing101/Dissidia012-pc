struct Screen503E0 {
    char pad_00[0xC];
    int pool;
    int block;
};

extern "C" {
extern char D_08A51C30[];
extern int func_0888204C(int size);
extern int func_0885C40C(int pool, int size, void *name, int flags);
}

extern "C" void func_08A503E0(Screen503E0 *self)
{
    int size = 0x270800;
    self->pool = func_0888204C(size);
    self->block = func_0885C40C(self->pool, size, D_08A51C30, 0x40);
}
