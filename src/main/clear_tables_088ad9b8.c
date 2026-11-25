extern void func_088CB110(void *);
extern void func_08993560(void *, int, int, void *);
extern char func_088AB6F4[];

typedef struct S98 {
    char pad0[0xF58];
    char sub[0x1DF88 - 0xF58];
    int a[0x80];
    int b[0x20];
    char pad1[0x1F784 - 0x1E208];
    int c[100];
    int d[100];
    int e[100];
    int f[100];
    char pad2[0x3F564 - 0x1FDC4];
    int g[0x20];
} S98;

S98 *func_088AD9B8(S98 *self)
{
    int i;
    func_088CB110(self->pad0 + 0xF58);
    for (i = 0; i < 0x80; i++) self->a[i] = 0;
    for (i = 0; i < 0x20; i++) self->b[i] = 0;
    for (i = 0; i < 100; i++) self->c[i] = 0;
    for (i = 0; i < 100; i++) self->d[i] = 0;
    for (i = 0; i < 100; i++) self->e[i] = 0;
    for (i = 0; i < 100; i++) self->f[i] = 0;
    func_08993560((char *)self + 0x3E8C8, 5, 0x280, func_088AB6F4);
    for (i = 0; i < 0x20; i++) self->g[i] = 0;
    return self;
}
