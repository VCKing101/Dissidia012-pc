/* Looks up a named entry, creates an object from it with func_0886D2C4, stores it at +0xC and applies func_089B0D40 to the object's own field +0xC when set. */
struct Ent { int a; int b; int c; };
struct Made { char pad[0xC]; void *inner; };
struct Obj { char pad[0xC]; struct Made *made; };
extern const char D_08B62F00[];
extern struct Ent *func_08869AB4(void *, void *);
extern struct Made *func_0886D2C4(const char *, int, int);
extern void func_089B0D40(void *, int, int);
void func_08B61EA0(struct Obj *self, void *a, void *b)
{
    struct Ent *e = func_08869AB4(a, b);
    void *inner;
    if (e == 0) {
        return;
    }
    self->made = func_0886D2C4(D_08B62F00, e->b, e->c);
    if (self->made == 0) {
        return;
    }
    inner = self->made->inner;
    if (inner == 0) {
        return;
    }
    func_089B0D40(inner, 0x20, 0);
}
