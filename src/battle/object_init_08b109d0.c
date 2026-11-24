/* Initialises an object: clears fields, zero-fills 0x20 bytes, allocates and constructs a 0x1C helper, copies a global. */
typedef struct Helper { char pad[0x1C]; } Helper;
typedef struct InitObj {
    char pad_00[0x20];
    int f20;
    int f24;
    char pad_28[0x8];
    Helper *f30;
    int f34;
    int f38;
    int f3c;
} InitObj;
typedef struct SubV { int v; } SubV;
typedef struct SaveBlk2 { char pad_00[0x25A8]; SubV sub; } SaveBlk2;
extern SaveBlk2 D_09AFB8A0;
extern void *func_08890900(unsigned size);
extern void func_08935828(void *p, int v, int n);
extern void func_08B113F8(void *p);
InitObj *func_08B109D0(InitObj *self, int a, int b)
{
    Helper *keep[1];
    Helper *p;
    Helper *r;
    int zero;
    SubV *sub = &D_09AFB8A0.sub;
    self->f20 = 0;
    self->f34 = a;
    self->f24 = 0;
    self->f38 = b;
    self->f3c = 1;
    func_08935828(self, 0, 0x20);
    zero = 0;
    keep[0] = (Helper *)zero;
    p = func_08890900(0x1C);
    r = keep[0];
    if (p != 0) {
        func_08B113F8(p);
        r = p;
    }
    self->f30 = r;
    self->f3c = sub->v;
    return self;
}
