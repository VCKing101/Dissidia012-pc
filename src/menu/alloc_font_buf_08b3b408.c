/* Allocates 0x20000 bytes (func_08B42B50) into self->0xBC and a named pool "SYSTEM_FONT_DUN" over it (func_0885C40C) into self->0xC0, then func_08B3B4DC(self). */
struct Self { char pad[0xBC]; int buf; int pool; };
extern char D_08B418BC[];
extern int func_08B42B50(int size);
extern int func_0885C40C(int buf, int size, char *name, int align);
extern void func_08B3B4DC(struct Self *self);
void func_08B3B408(struct Self *self)
{
    int size = 0x20000;
    self->buf = func_08B42B50(size);
    self->pool = func_0885C40C(self->buf, size, D_08B418BC, 0x10);
    func_08B3B4DC(self);
}
