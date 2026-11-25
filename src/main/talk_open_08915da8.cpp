struct Sub08915DA8 { char pad[8]; int v; };
struct Obj08915DA8 { char pad[0x380]; unsigned flags; };
struct Root08915DA8 {
    char pad0[0x68];
    int state;
    char pad1[0x90 - 0x6C];
    int sel;
    char pad2[0x6D0 - 0x94];
    Obj08915DA8 *o6d0;
    char pad3[0x704 - 0x6D4];
    Obj08915DA8 *slots[1];
    char pad4[0x744 - 0x708];
    Sub08915DA8 *sub;
    char pad5[0x748 - 0x748];
    int a748;
    int a74c;
    int a750;
    int a754;
    char pad6[0x770 - 0x758];
    char path[0x65];
    unsigned char b7d5;
    unsigned char b7d6;
    unsigned char b7d7;
};
extern "C" void func_089359A4(void *, int);
extern "C" void func_08914B64(Root08915DA8 *, int);
extern "C" void func_08A64054(int, int);
extern "C" int func_089359E8(void *);
extern "C" int func_0885C40C(int, int, char *, int);
extern "C" int func_0885C6CC(int, int, int, int, int, int);
extern "C" void func_088FE850(int, int, int, void (*)(void), Root08915DA8 *, int);
extern "C" void func_08917274(void);
extern char D_089CE3C4[];
struct Nz08915DA8 {
    static unsigned char nz(int v) { return v != 0; }
};
extern "C" void func_08915DA8(Root08915DA8 *self, int name, unsigned char flag);
extern "C" void func_08915DA8(Root08915DA8 *self, int name, unsigned char flag)
{
    unsigned busy0 = self->sub->v != 0; unsigned char busy = busy0;
    if (busy) {
        return;
    }
    if (self->state == 3) {
        return;
    }
    func_089359A4(self->path, name);
    self->b7d7 = 0;
    func_08914B64(self, 3);
    self->b7d6 = flag;
    if (self->b7d6 == 0) {
        self->o6d0->flags = self->o6d0->flags | 2;
    }
    if (self->sel >= 0) {
        func_08A64054(*(int *)((char *)self->slots[self->sel] + 0x5C4), 1);
    }
    if (func_089359E8(self->path) != 0) {
        int m = func_0885C40C(self->a750, 0xC8000, D_089CE3C4, 0x10);
        self->a754 = m;
        int a = func_0885C6CC(m, 0x40000, 0x40, 0, 1, 0);
        self->a748 = a;
        int b = func_0885C6CC(self->a754, 0x5000, 0x40, 0, 1, 0);
        self->a74c = b;
        self->b7d5 = 0;
        func_088FE850(name, b, 0x5000, func_08917274, self, 0);
    }
}
