struct Work30480 {
    void *table_00;
    void *table_04;
    char pad_08[0x6A8 - 8];
    int step;
    char pad_6AC[0x6D5 - 0x6AC];
    unsigned char byte_6D5;
    unsigned char byte_6D6;
};

struct Screen30480 {
    char pad_00[0x6DC];
    Work30480 *work;
};

struct Owner30480 {
    char pad_000[0x1299];
    unsigned char flag;
};

struct Menu30480 {
    char pad_00[0x14C];
    Owner30480 *owner;
};

extern "C" {
extern int D_089D69E8;
extern Menu30480 *D_08A457EC;
extern char D_08A3556C[];
extern void func_088854C4(int o, int kind, int id);
extern void func_08885388(int o);
extern void func_088D3B88(void *a, void *b, void *table, int c, int d);
extern int func_08A41098(Menu30480 *m);
extern void func_08A301B0(Screen30480 *self, int a);
extern void func_08A4185C(Menu30480 *m, int a);
}

extern "C" void func_08A30480(Screen30480 *self)
{
    int kind = 0x11;
    func_088854C4(D_089D69E8, kind, 0x4005);
    func_08885388(D_089D69E8);
    func_088D3B88(self->work->table_00, self->work->table_04, D_08A3556C, 0, 0x120);
    self->work->byte_6D5 = D_08A457EC->owner->flag;
    self->work->byte_6D6 = func_08A41098(D_08A457EC);
    func_08A301B0(self, 0);
    func_08A301B0(self, 1);
    func_08A4185C(D_08A457EC, 0);
    self->work->step = 2;
}
