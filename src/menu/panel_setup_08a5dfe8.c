/* Stores the setup arguments, runs the panel's init steps and picks the starting mode by whether an entry is selected. */
typedef struct Ps { char pad0[0x28]; int mode; char pad1[0x40 - 0x2C]; int f40; char pad2[0x150 - 0x44]; int f150, f154, f158, f15C, f160; char pad3[0x16C - 0x164]; int sel; } Ps;
extern int func_088729A0(int);
extern void func_088D9F5C(int, int, int);
extern void func_08A5E7EC(Ps *, int);
extern void func_08A5EA48(Ps *, int);
extern void func_08A5EAA8(Ps *);
extern void func_08A5EB20(Ps *);
extern void func_08A5EBA4(Ps *);
extern void func_08A5EC78(Ps *);
extern void func_08A5ECCC(Ps *);
extern void func_08A5ED84(Ps *);
extern void func_08A5EDD8(Ps *);
void func_08A5DFE8(Ps *self, int a, int b, int c, int d, int e) {
    int has;
    self->f154 = a;
    self->f160 = b;
    self->f158 = c;
    self->f15C = d;
    self->f150 = func_088729A0(0);
    has = 0;
    if (self->sel != -1) has = 1;
    func_08A5EDD8(self);
    func_08A5EAA8(self);
    func_08A5E7EC(self, e);
    if (has) {
        func_08A5EA48(self, 1);
    } else {
        func_08A5EA48(self, 0);
    }
    func_08A5EBA4(self);
    func_08A5EC78(self);
    func_08A5ECCC(self);
    func_08A5ED84(self);
    func_08A5EB20(self);
    if (has) {
        func_088D9F5C(self->f40, 1, 1);
        self->mode = 2;
    } else {
        self->mode = 1;
    }
}
