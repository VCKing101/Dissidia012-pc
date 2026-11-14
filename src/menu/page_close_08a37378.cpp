struct Work37378 {
    void *table_00;
    void *table_04;
    int handle_08;
    char pad_0C[0x6C - 0xC];
    int index;
    char pad_70[0xDB4 - 0x70];
    unsigned char flag_DB4;
    unsigned char flag_DB5;
    unsigned char flag_DB6;
};

struct Screen37378 {
    char pad_00[0xF64];
    Work37378 *work;
};

extern "C" {
extern int D_089D69E8;
extern char D_08A38F34[];
extern unsigned short D_08A38FF0[];
extern void func_088D3EA0(void *a, void *b, void *table);
extern void func_08A37E10(Screen37378 *self, int a);
extern void func_08A37180(Screen37378 *self);
extern void func_088D9CD8(int o);
extern void func_0888550C(int o, int handle, int a, int id);
}

extern "C" void func_08A37378(Screen37378 *self)
{
    func_088D3EA0(self->work->table_00, self->work->table_04, D_08A38F34);
    self->work->flag_DB4 = 0;
    self->work->flag_DB5 = 0;
    self->work->flag_DB6 = 0;
    func_08A37E10(self, 0);
    func_08A37180(self);
    func_088D9CD8(*(int *)((char *)self->work->table_04 + 4));
    func_0888550C(D_089D69E8, self->work->handle_08, 0, D_08A38FF0[self->work->index]);
}
