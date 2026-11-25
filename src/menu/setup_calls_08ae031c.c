typedef struct In { char p[0x434]; int f434; char q[0x5BC-0x438]; int f5BC; } In;
typedef struct Obj { char p[0x850]; In *in; } Obj;
extern void func_08AE09D8(Obj *, int, int);
extern void func_08AE0AB4(Obj *, int, int);
extern int func_08AE16C4(Obj *);
extern void func_088D9CFC(int);
extern void func_08AE06B8(Obj *);
extern void func_08AE07D0(Obj *);
extern void func_08AE22E8(Obj *);

void func_08AE031C(Obj *self)
{
    int t[2];
    int m1 = -1;
    func_08AE09D8(self, 0x9, -1);
    func_08AE09D8(self, 0xa, -1);
    func_08AE09D8(self, 0xb, -1);
    func_08AE09D8(self, 0xc, -1);
    func_08AE09D8(self, 0x13, -1);
    func_08AE09D8(self, 0x12, -1);
    func_08AE09D8(self, 0x11, -1);
    func_08AE09D8(self, 0x10, -1);
    func_08AE09D8(self, 0x14, -1);
    func_08AE09D8(self, 0x16, -1);
    func_08AE09D8(self, 0x17, -1);
    func_08AE09D8(self, 0x1b, -1);
    func_08AE09D8(self, 0x19, -1);
    func_08AE09D8(self, 0x1a, -1);
    func_08AE09D8(self, 0x23, -1);
    func_08AE09D8(self, 0x24, -1);
    func_08AE09D8(self, 0x25, -1);
    func_08AE09D8(self, 0x26, -1);
    func_08AE09D8(self, 0x1c, -1);
    func_08AE09D8(self, 0x1d, -1);
    func_08AE09D8(self, 0x1e, -1);
    func_08AE09D8(self, 0x1f, -1);
    func_08AE09D8(self, 0x20, -1);
    func_08AE09D8(self, 0x21, -1);
    func_08AE09D8(self, 0x22, -1);
    func_08AE09D8(self, 0x27, -1);
    func_08AE09D8(self, 0x28, -1);
    func_08AE09D8(self, 0x29, -1);
    func_08AE09D8(self, 0x2a, -1);
    func_08AE09D8(self, 0x2b, -1);
    func_08AE09D8(self, 0x2c, -1);
    func_08AE09D8(self, 0x2d, -1);
    func_08AE09D8(self, 0x2e, -1);
    func_08AE09D8(self, 0x2f, -1);
    func_08AE09D8(self, 0x15, self->in->f5BC + 0x29);
    func_08AE09D8(self, 0xE, 0x25);
    func_08AE0AB4(self, 0xc, -1);
    func_08AE0AB4(self, 0xd, -1);
    func_08AE0AB4(self, 0xf, -1);
    func_08AE0AB4(self, 0x10, -1);
    func_08AE0AB4(self, 0xe, -1);
    t[0] = func_08AE16C4(self);
    t[1] = m1;
    if (t[0] != m1) {
        t[1] = t[0] + 0x45;
        func_08AE09D8(self, 0xD, t[1]);
    } else {
        func_088D9CFC(self->in->f434);
    }
    func_08AE06B8(self);
    func_08AE07D0(self);
    func_08AE22E8(self);
}
