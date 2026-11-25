typedef struct Obj { int a0; int a4; int a8; int h; } Obj;
extern int D_089D7830;
extern void func_088D6DBC(Obj *o);
extern int func_088D0700(int g, unsigned char a, int b, int c);
extern int func_088D7720(Obj *o, int a, int b, int c, int d, int e, int f);
int func_088D6E24(Obj *o, int p1, int p2, int p3, int p5, int p6, int p7, int p8, unsigned char x0, unsigned char x4)
{
    int r;
    unsigned char c;
    func_088D6DBC(o);
    if (p1 == 0) return -1;
    int g;
    g = D_089D7830;
    c = p5 & 0xFF;
    o->h = func_088D0700(g, c, p6, p7);
    if (o->h == 0) return -2;
    r = func_088D7720(o, p1, p2, p3, p8, x0, x4);
    if (r != 0) return -3;
    return 0;
}
