/* Reads the time words with func_089C5D98 into a local and stores them (halfword, byte, byte) at D_089D712C + 0x1DF80; copies D_089D7F14/16 to +0x1DF3C/3E. */
typedef struct Tm088f1b8c { unsigned short a; unsigned short b; unsigned short c; unsigned short rest[5]; } Tm088f1b8c;
typedef struct Dst088f1b8c { unsigned short a; unsigned char b; unsigned char c; } Dst088f1b8c;
typedef struct Pos088f1b8c { unsigned char pad[0x5C]; short x; short y; } Pos088f1b8c;
extern char *D_089D712C;
extern short D_089D7F14;
extern short D_089D7F16;
extern void func_089C5D98(Tm088f1b8c *t);
void func_088F1B8C(void)
{
    Tm088f1b8c t;
    Dst088f1b8c *d;
    Pos088f1b8c *p;
    func_089C5D98(&t);
    d = (Dst088f1b8c *)(D_089D712C + 0x1DF80);
    d->a = t.a;
    d->b = t.b;
    d->c = t.c;
    p = (Pos088f1b8c *)(D_089D712C + 0x1DEE0);
    p->x = D_089D7F14;
    p->y = D_089D7F16;
}
