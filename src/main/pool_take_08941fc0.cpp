/* Takes the first free pool entry (state 0) for the request: sets it up (virtual 4 on the entry), records the handle (virtual 10 on the pool), the request and bumps the use count. */
struct Elem08941FC0 {
    char d[4];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(int a);
};
struct Pool08941FC0 {
    Elem08941FC0 **list; char p1[4]; int *a; int *b; int *c; int count; char p2[0x24 - 0x18];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9();
    virtual int v10(int x);
    virtual int v11(int x);
};
extern "C" {
int func_08941FC0(Pool08941FC0 *self, int x, int y)
{
    if (self->v11(y) == 0) {
        return -1;
    }
    {
        int i;
        for (i = 0; i < self->count; i++) {
            if (*(int *)self->list[i] == 0) {
                self->list[i]->v4(y);
                self->a[i] = self->v10(y);
                self->b[i] = y;
                self->c[i] = self->c[i] + 1;
                return i;
            }
        }
    }
    return -1;
}
}
