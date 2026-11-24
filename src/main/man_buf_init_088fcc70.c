/* Sets fields 0x14/0x18 to 1, registers the "MAN::BUF" block (func_0885C40C(a, b, name, 0x40)) into +0x20, stores c at +0x24, 0 at +0x1C, d at +0x28. */
typedef struct Obj088fcc70 { unsigned char pad[0x14]; int f14, f18, f1c; void *buf; int f24, f28; } Obj088fcc70;
extern char D_089CD070[];
extern void *func_0885C40C(int a, int b, char *name, int align);
void func_088FCC70(Obj088fcc70 *self, int a, int b, int c, int d)
{
    self->f14 = 1;
    self->f18 = 1;
    self->buf = func_0885C40C(a, b, D_089CD070, 0x40);
    self->f24 = c;
    self->f1c = 0;
    self->f28 = d;
}
