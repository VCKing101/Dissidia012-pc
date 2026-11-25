typedef struct Sc { char pad[0x54]; int f54; int f58; int f5C; int f60; int f64; } Sc;
extern int D_089D6998;
extern char *D_089D76F0;
extern void func_0887BA5C(int);
extern int func_08A5738C(Sc *, int);
extern int func_08A573A0(Sc *, int);
extern int func_0887B500(int, int, int);
extern int func_0887D868(int, int, int, int, int, int, int, int);
extern int func_0887E534(int);
extern void func_088D428C(Sc *, void *, void *, int);
extern void func_08A55174(Sc *);
void func_08A56F60(Sc *self, short id, int h0)
{
    int h;
    unsigned char c;
    unsigned char u;
    short sid = id;
    int idx = sid;
    if (self->f5C != 0) {
        func_0887BA5C(self->f5C);
        self->f5C = 0;
    }
    if (self->f60 != 0) {
        func_0887BA5C(self->f60);
        self->f60 = 0;
    }
    if (self->f64 != 0) {
        func_0887BA5C(self->f64);
        self->f64 = 0;
    }
    c = func_08A5738C(self, idx);
    u = func_08A573A0(self, idx);
    h = 0;
    if (c == 0) {
        h = func_0887B500(self->f54, 8, 0x2D);
    } else if (c == 1) {
        h = h0;
    } else if (c == 2) {
        short *p = (short *)(D_089D76F0 + u * 0x13C);
        if (p != 0)
            h = func_0887B500(self->f54, 0xC, *p);
    }
    if (h == 0)
        return;
    {
        int v = func_0887D868(D_089D6998, 0, h, -1, 0, 0, 0, 0);
        self->f5C = v;
        v = func_0887E534(v);
        self->f60 = v;
        v = func_0887E534(self->f5C);
        self->f64 = v;
        func_088D428C(self, &self->f5C, (char *)self + 0x8C, 1);
        func_08A55174(self);
    }
}
