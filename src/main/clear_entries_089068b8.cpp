// Clears the state fields of every entry the owned container reports.
struct E { char d[0xC]; int a, b, c; char f; char pad[3]; int g, h, i; };
struct P { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual E *v8(int i); virtual void v9(); virtual int v10(); };
struct S { char d[0x2D0]; P *p; };
extern "C" void func_089068B8(S *self)
{
    if (self->p) {
        int n = self->p->v10();
        int i;
        for (i = 0; i < n; i++) {
            E *e = self->p->v8(i);
            e->a = 0; e->b = 0; e->c = 0; e->f = 0; e->g = 0; e->h = 0; e->i = 0;
        }
    }
}
