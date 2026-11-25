/* Computes a vector for the object at +4 and a1, copies it twice, and stores func_089A37DC of the copy at +0x10 of the object. */
typedef struct V3 { float x, y, z; } V3;
typedef struct Obj { char p[0x10]; float len; } Obj;
typedef struct S { char p[4]; Obj *obj; } S;
extern void func_089A3528(V3 *out, Obj *o, int a);
extern float func_089A37DC(V3 *v);
void func_089254E0(S *self, int a) {
    V3 c;
    V3 b;
    V3 v;
    V3 unused;
    func_089A3528(&v, self->obj, a);
    b = v;
    c = b;
    self->obj->len = func_089A37DC(&c);
}
