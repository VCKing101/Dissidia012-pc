/* Allocates 0x270800 bytes (func_0888204C) into self->0 and creates a named pool "DUNGEON__MEMORY" over it into self->4. */
struct Self { int buf; int pool; };
extern char D_08B42C18[];
extern int func_0888204C(int size);
extern int func_0885C40C(int buf, int size, char *name, int align);
void func_08B42AB0(struct Self *self)
{
    int size = 0x270800;
    self->buf = func_0888204C(size);
    self->pool = func_0885C40C(self->buf, size, D_08B42C18, 0x10);
}
