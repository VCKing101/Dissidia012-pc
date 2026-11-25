typedef struct Obj30DB8 { char pad[0x20]; int h20; char pad1[0x178 - 0x24]; int f178; char pad2[0x198 - 0x17C]; int h198; } Obj30DB8;
typedef struct Self30DB8 { char pad[4]; int f4; char pad1[0x38 - 8]; int h38; char pad2[0x48 - 0x3C]; Obj30DB8 *obj; } Self30DB8;
typedef struct G5EC0 { char pad[0x710]; int h710; } G5EC0;
extern int D_089D4960;
extern G5EC0 *D_089D5EC0;
extern int D_089D6D80;
extern int D_089D6DC8;
extern int D_089D4DC8;
extern char D_08A35950[];
extern char D_08A35958[];
extern char D_08A3597C[];
extern void func_08A323C4();
extern void func_08A3244C();
extern void func_08A3246C();
extern int func_0885C6CC();
extern void func_08844588();
extern void func_08841EB4();
extern void func_08841CC0();
extern void func_088A5A98();
extern void func_088A9814();
extern void func_088A5D78();
extern void func_088A94F0();
extern void func_08865088();
extern void func_0885A290();
extern void func_088CB948();
extern void func_088823C4();

void func_08A30DB8(Self30DB8 *self)
{
    Obj30DB8 *o;
    int one = 1;
    o = (Obj30DB8 *)func_0885C6CC(self->h38, 0x1B0, 0, 0, one, 0);
    self->obj = o;
    o->f178 = 0;
    func_08844588(D_089D4960);
    self->obj->h20 = func_0885C6CC(D_089D5EC0->h710, 0x270800, 0, 0, one, 0);
    func_08841EB4(self->h38);
    func_08841CC0(self->f4, 0, 30000, 0, 40000, 0, 70000, 0, 69000, 0, 73000, 0, 31000, 0, 0x797C, 0, 58000, 0, 0x18A88);
    func_088A5A98(self->f4, 0, 0, 0xDB24, 0xDB88, 0xE808, 0x1101C, 80000, 100000, 0x10FE0, 0x139AC);
    func_088A9814(self->f4, 0, 56000);
    func_088A5D78(D_089D6D80, 0x8866528, 0x8866560, 0, 0, 0);
    func_088A94F0(D_089D6DC8, 0);
    func_08865088(&self->f4, 0, 0, func_08A323C4, D_08A35950);
    func_0885A290(D_089D4DC8, self->h38);
    self->obj->h198 = func_0885C6CC(self->h38, 0x5000, 0, 0, one, 0);
    func_088CB948(self->obj->h198, 0x5000);
    func_088823C4(self->f4, 0, 0xD1F6, func_08A3244C, D_08A35958);
    func_088823C4(self->f4, 0, 83000, func_08A3246C, D_08A3597C);
}
