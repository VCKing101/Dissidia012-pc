/* Reads 4 bytes with func_0890115C into a local; on success stores it at 0, sets bit 0 of field 0x1C and field 0x40 = func_089C5FB8() & 0x7FFFFFFF; on failure stores -1 at 0. */
typedef struct Obj08900b7c { int first; unsigned char pad0[0x1C - 4]; unsigned flags; unsigned char pad1[0x40 - 0x20]; unsigned id; } Obj08900b7c;
extern int func_0890115C(void *dst, int n);
extern unsigned func_089C5FB8(void);
void func_08900B7C(Obj08900b7c *self)
{
    int buf[4];
    if (func_0890115C(buf, 4) != 0) {
        self->first = buf[0];
        self->flags = self->flags | 1;
        self->id = func_089C5FB8() & 0x7FFFFFFF;
    } else {
        self->first = -1;
    }
}
