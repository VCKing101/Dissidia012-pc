// Constructor: runs the base constructor, installs vtable 08B43668, clears +0x1C/+0x20 and stores func_088BBD8C(src->+0x1C) at +0x18.
extern void func_08B44B78(void *);
extern int func_088BBD8C(int);
extern char D_08B43668[];
typedef struct Obj57 {
    char pad0[0x14];
    void *vtbl;
    int f18;
    int f1C;
    float f20;
} Obj57;
typedef struct Src57 {
    char pad0[0x1C];
    int f1C;
} Src57;
Obj57 *func_08B42FB8(Obj57 *self, int unused, Src57 *src)
{
    Src57 *s = src;
    Obj57 *o = self;
    func_08B44B78(o);
    o->vtbl = D_08B43668;
    o->f1C = 0;
    o->f20 = 0.0f;
    o->f18 = func_088BBD8C(s->f1C);
    return o;
}
