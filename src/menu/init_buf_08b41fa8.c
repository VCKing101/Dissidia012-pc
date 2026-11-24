/* Allocates 0x400 bytes (func_088820D8) into self->0, inits self+4 with it (func_0886504C), sets self+0x2C/0x30 to -1 and registers "Update" via func_08865088(self+4, -1, 1000, func_08B41EA0, name). */
struct Self { int buf; char h[0x28]; int a; int b; };
extern char D_08B42080[];
extern int func_088820D8(int size);
extern void func_0886504C(char *h, int buf, int size);
extern void func_08865088(char *h, int a, int ms, void *fn, char *name);
extern void func_08B41EA0(void);
void func_08B41FA8(struct Self *self)
{
    char *h;
    self->buf = func_088820D8(0x400);
    h = self->h;
    func_0886504C(h, self->buf, 0x400);
    self->a = -1;
    self->b = -1;
    func_08865088(h, -1, 1000, func_08B41EA0, D_08B42080);
}
