typedef struct Task { char p0[4]; struct Rec *rec; } Task;
typedef struct Rec { char p0[0x15]; unsigned char b15; unsigned char b16; } Rec;
typedef struct Job { char p0[4]; Rec *rec; } Job;
extern void func_088CC914(unsigned char *s);
extern Rec *func_088D0700(int h, int a, int b, int c);
extern Task *func_088823C4(int a, int b, int c, void *fn, const char *name);
extern void func_088CFF30(int h, Rec *r);
extern void func_088CCB18(void);
extern int D_089D7830;
extern char D_089CBCF4[];
int func_088CCA0C(unsigned char *self, int p1, int p2, int p3, int p5, int p6, int p7)
{
    Rec *r;
    unsigned char c;
    Rec *q;
    Task *t;
    func_088CC914(self);
    if (p1 == 0) return -1;
    c = p5;
    p5 = c;
    r = func_088D0700(D_089D7830, (unsigned char)p5, p6, p7);
    ((Job *)self)->rec = r;
    if (r == 0) return -2;
    t = func_088823C4(p1, p2, p3, (void *)func_088CCB18, D_089CBCF4);
    if (t == 0) return -3;
    t->rec = (Rec *)self;
    ((Job *)self)->rec->b16 = 2;
    ((Job *)self)->rec->b15 = 200;
    q = ((Job *)self)->rec;
    func_088CFF30(D_089D7830, q);
    *self = 1;
    return 0;
}
