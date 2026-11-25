typedef struct Op { int f0; int f4; } Op;
typedef struct Sb { char pad[0x24]; int f24; int f28; char pad2C[8]; int f34; char pad38[4]; int f3C; } Sb;
typedef struct P3 { char pad0[0x24C]; Op *f24C; Sb s; char pad290[4]; unsigned int f294; } P3;
typedef struct P4 { char pad[0x44]; int f44; } P4;
extern void func_088DA0A8(int, int, char *, char *);
extern void func_088D9B84(int, char *);
extern void func_088D9AF4(int, char *);
void func_08A94A2C(int a1, P3 *p3, P4 *p4)
{
    char b[64];
    char c[4];
    Sb *s;
    Op *o;
    int k;
    o = p3->f24C;
    s = &p3->s;
    func_088DA0A8(p4->f44, 2, b, c);
    func_088D9B84(*(int *)((char *)p4 + (s->f24 + o->f4) * 4), b);
    func_088D9AF4(*(int *)((char *)p4 + (s->f24 + o->f4) * 4), c);
    func_088D9B84(*(int *)((char *)p4 + (s->f28 + o->f4) * 4), b);
    func_088D9AF4(*(int *)((char *)p4 + (s->f28 + o->f4) * 4), c);
    k = o->f4;
    if ((p3->f294 & 0x100) != 0) {
        func_088D9B84(*(int *)((char *)p4 + (s->f28 + s->f3C + k) * 4), b);
        func_088D9AF4(*(int *)((char *)p4 + (s->f28 + s->f3C + o->f4) * 4), c);
        func_088D9B84(*(int *)((char *)p4 + (s->f28 + s->f3C * 2 + o->f4) * 4), b);
        func_088D9AF4(*(int *)((char *)p4 + (s->f28 + s->f3C * 2 + o->f4) * 4), c);
        k = o->f4;
    }
    func_088D9B84(*(int *)((char *)p4 + (s->f34 + k) * 4), b);
    func_088D9AF4(*(int *)((char *)p4 + (s->f34 + o->f4) * 4), c);
}
