// Constructor: base init func_08B44B78, vtable D_08B42EF8 at +0x14, fields cleared, array of 8 x 0x30 at +0x50 built with func_08993560.
extern char D_08B42EF8[];
extern void func_08B422E0(void);
extern void func_08B44B78(void *);
extern void func_08993560(void *, int, int, void (*)(void));
typedef struct { char pad0[0x14]; void *vt; char pad1[0x40 - 0x18]; int a, b, c, d; char arr[1]; } Obj08B422F0;
Obj08B422F0 *func_08B422F0(Obj08B422F0 *self)
{
    int one;
    func_08B44B78(self);
    self->vt = D_08B42EF8;
    self->a = 0;
    self->b = 0;
    one = 1;
    self->c = 0;
    self->d = one;
    func_08993560(self->arr, 8, 0x30, func_08B422E0);
    return self;
}
