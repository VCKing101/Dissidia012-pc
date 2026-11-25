// Fills an object from a source buffer: returns -1 on bad args, -2 without a handle, -3 on parse failure, else 0.
struct O08AB3D9C { char kind; char pad[3]; float v; unsigned color; };
struct T08AB3D9C { int a, b, c; };
extern void func_08AB3D80(struct O08AB3D9C *o);
extern void *func_0889381C(void *);
extern void func_08AB3A40(struct T08AB3D9C *t);
extern int func_08AB3AFC(struct T08AB3D9C *t, void *a, int b, void *c);
extern float func_08AB3C54(struct T08AB3D9C *t);
extern void func_08AB3A78(struct T08AB3D9C *t, int n);
extern void *D_089D6AC8;
int func_08AB3D9C(struct O08AB3D9C *self, void *src, int n)
{
    struct T08AB3D9C t;
    void *h;
    func_08AB3D80(self);
    if (src == 0 || n <= 0) {
        return -1;
    }
    h = func_0889381C(D_089D6AC8);
    if (h == 0) {
        return -2;
    }
    func_08AB3A40(&t);
    if (func_08AB3AFC(&t, src, n, h) != 0) {
        func_08AB3A78(&t, 2);
        return -3;
    }
    self->v = func_08AB3C54(&t);
    self->color = 0xFF808080;
    self->kind = 1;
    func_08AB3A78(&t, 2);
    return 0;
}
