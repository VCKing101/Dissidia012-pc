/* Stores four setup values, resets the object, opens a handle from the first two and registers the object's block. */
struct Obj { char pad0[0x30]; int b30[2]; int h38; char pad1[4]; int a40; int a44; char pad2[4]; int a4c; int a50; };
extern void func_08B4C6FC(struct Obj *);
extern int func_0887B3EC(int, int, int, int, int, int);
extern void func_08B4C66C(struct Obj *, int);
extern void func_088D428C(void *, void *, void *, int);
extern char D_08B4C7B0[];
void func_08B4C5D4(struct Obj *self, int a, int b, int start, int extra) {
    self->a40 = a;
    self->a44 = b;
    self->a4c = start;
    self->a50 = extra + start;
    func_08B4C6FC(self);
    self->h38 = func_0887B3EC(a, b, 0, 3, 0, 0);
    func_08B4C66C(self, self->a4c);
    func_088D428C(&self->b30[0], &self->h38, D_08B4C7B0, 1);
}
