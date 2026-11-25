typedef struct In { int w[0x8000]; } In;
typedef struct Obj { char p[0x138]; In *in; } Obj;
#define W(o) (self->in->w[(o) / 4])
#define B(o) ((char *)self->in + (o))
extern int D_089D6998;
extern unsigned short D_08B1C55A[];
extern unsigned char D_08B1C930[], D_08B1C8EC[];
extern void func_0887BA5C(int);
extern void func_0893476C(void *, void *, ...);
extern void func_0887DF14(int, void *, int, void *);
extern int func_0887D868(int, int, void *, int, int, int, int, int);
extern int func_0887E534(int);
extern int func_08B0C4B8(Obj *);

void func_08B124BC(Obj *self)
{
    char fmt[64];
    int h;
    int a;
    int b;
    char *dst;
    if (W(0xb80) != 0) {
        func_0887BA5C(W(0xb80));
        W(0xb80) = 0;
    }
    if (W(0xb84) != 0) {
        func_0887BA5C(W(0xb84));
        W(0xb84) = 0;
    }
    if (W(0xb98) != 0) {
        func_0887BA5C(W(0xb98));
        W(0xb98) = 0;
    }
    if (W(0xb9c) != 0) {
        func_0887BA5C(W(0xb9c));
        W(0xb9c) = 0;
    }
    if (W(0xba0) != 0) {
        func_0887BA5C(W(0xba0));
        W(0xba0) = 0;
    }
    if (W(0xba4) != 0) {
        func_0887BA5C(W(0xba4));
        W(0xba4) = 0;
    }
    func_0893476C(fmt, D_08B1C930, W(0x1F468));
    dst = B(0x263A);
    func_0887DF14(D_089D6998, dst, 0x80, fmt);
    {
        In *q = self->in;
        h = func_0887D868(D_089D6998, 0, (char *)q + 0x263A, -1, 0, 0, 0, 0);
    }
    self->in->w[D_08B1C55A[0x2C] + 0x221] = h;
    b = W(0x1F468) * W(0x1F470);
    func_0893476C(fmt, D_08B1C8EC, b);
    dst = B(0x26BA);
    func_0887DF14(D_089D6998, dst, 0x80, fmt);
    dst = B(0x26BA);
    h = func_0887D868(D_089D6998, 0, dst, -1, 0, 0, 0, 0);
    self->in->w[D_08B1C55A[0x2D] + 0x221] = h;
    a = func_08B0C4B8(self);
    if (W(0x1F8C0) == 0) {
        b = a - b;
    } else {
        b = b + a;
    }
    if (b > 99999999) {
        b = 99999999;
    }
    func_0893476C(fmt, D_08B1C8EC, b);
    dst = B(0x273A);
    func_0887DF14(D_089D6998, dst, 0x80, fmt);
    dst = B(0x273A);
    h = func_0887D868(D_089D6998, 0, dst, -1, 0, 0, 0, 0);
    self->in->w[D_08B1C55A[0x2E] + 0x221] = h;
    W(0xB84) = func_0887E534(W(0xB80));
    W(0xB9C) = func_0887E534(W(0xB98));
    W(0xBA4) = func_0887E534(W(0xBA0));
}
