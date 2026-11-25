/* Copies the position vector of the owner's object twice and passes the copy to func_08921D74. */
typedef struct V3 { float x, y, z; } V3;
typedef struct In { char p[0xC]; void *q; } In;
typedef struct S { char p[0xC]; In *in; } S;
extern V3 *func_08925384(void *o);
extern void func_08921D74(S *self, V3 *v);
void func_089212F4(S *self) {
    V3 b;
    V3 a;
    V3 *p = func_08925384(self->in->q);
    a = *p;
    b = a;
    func_08921D74(self, &b);
}
