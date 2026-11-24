/* Calls func_088F4638(0), reads the time into a local with func_089C5D98, stores the constant date 0x7D8/12/18 at D_089D712C + 0x1DE7C and the read time (halfword, byte, byte) at +0x1DE80. */
typedef struct Tm088f43dc { unsigned short a; unsigned short b; unsigned short c; unsigned short rest[5]; } Tm088f43dc;
typedef struct Dst088f43dc { unsigned short a; unsigned char b; unsigned char c; } Dst088f43dc;
extern char *D_089D712C;
extern void func_088F4638(int v);
extern void func_089C5D98(Tm088f43dc *t);
void func_088F43DC(void)
{
    Tm088f43dc t;
    Dst088f43dc *d;
    func_088F4638(0);
    func_089C5D98(&t);
    d = (Dst088f43dc *)(D_089D712C + 0x1DE7C);
    d->a = 0x7D8;
    d->b = 12;
    d->c = 18;
    d = (Dst088f43dc *)(D_089D712C + 0x1DE80);
    d->a = t.a;
    d->b = t.b;
    d->c = t.c;
}
