/* Allocates 0x270800 bytes (func_0888204C) into self->0x134 and creates a named pool "CROOM_T_BUF" over it into self->0x138. */
struct Self { char pad[0x134]; int buf; int pool; };
extern char D_08B29BF0[];
extern int func_0888204C(int size);
extern int func_0885C40C(int buf, int size, char *name, int align);
void func_08B28C14(struct Self *self)
{
    int size = 0x270800;
    self->buf = func_0888204C(size);
    self->pool = func_0885C40C(self->buf, size, D_08B29BF0, 0x40);
}
