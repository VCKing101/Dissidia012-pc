struct Screen42A78 {
    char pad_00[0x134];
    int block;
    int pool;
};

extern "C" {
extern char D_08A45660[];
extern int func_0888204C(int size);
extern int func_0885C40C(int pool, int size, void *name, int flags);
}

extern "C" void func_08A42A78(Screen42A78 *self)
{
    int size = 0x270800;
    self->pool = func_0888204C(size);
    self->block = func_0885C40C(self->pool, size, D_08A45660, 0x40);
}
