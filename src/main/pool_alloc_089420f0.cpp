/* Allocates a pool slot: checks the request (virtual 11), reuses a matching entry or takes the first free one (state 0), sets it up and records a handle with no mark. */
struct Elem089420F0 {
    char d[4];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(int a);
};
struct Pool089420F0 {
    Elem089420F0 **list; char p1[4]; int *a; int *b; int *c; int count; char p2[0x24 - 0x18];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9();
    virtual int v10(int x);
    virtual int v11(int x);
};
extern "C" {
int func_08941D40(Pool089420F0 *p, int x);
int func_089420F0(Pool089420F0 *self, int x, int y)
{
    int r;
    if (self->v11(y) == 0) {
        return -1;
    }
    r = func_08941D40(self, x);
    if (r != -1) {
        return r;
    }
    {
        int i;
        for (i = 0; i < self->count; i++) {
            if (*(int *)self->list[i] == 0) {
                self->list[i]->v4(y);
                self->a[i] = self->v10(y);
                self->b[i] = 0;
                self->c[i] = self->c[i] + 1;
                return i;
            }
        }
    }
    return -1;
}
}
