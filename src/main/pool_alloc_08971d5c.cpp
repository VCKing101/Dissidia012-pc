/* Allocates a pool slot: checks the request (virtual 11), reuses a matching entry or takes the first free one (state 0), sets it up and records a handle. */
struct Elem08971D5C {
    char d[4];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(int a);
    int state() { return *(int *)this; }
};
struct Pool08971D5C {
    Elem08971D5C **list; char p1[4]; int *a; int *b; int *c; int count; char p2[0x24 - 0x18];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9();
    virtual int v10(int x);
    virtual int v11(int x);
};
struct PoolD08971D5C : Pool08971D5C { int arg; };
extern "C" {
int func_08941D40(PoolD08971D5C *p, int x);
void func_08971554(Elem08971D5C *e, int arg);
int func_08971D5C(PoolD08971D5C *self, int x, int y)
{
    int r;
    if (self->v11(y) == 0) {
        return -1;
    }
    r = func_08941D40(self, x);
    if (r == -1) {
        int i;
        for (i = 0; i < self->count; i++) {
            if (*(int *)self->list[i] == 0) {
                self->list[i]->v4(y);
                self->a[i] = self->v10(y);
                self->b[i] = 0;
                self->c[i] = self->c[i] + 1;
                func_08971554(self->list[i], self->arg);
                return i;
            }
        }
    }
    return r;
}
}
