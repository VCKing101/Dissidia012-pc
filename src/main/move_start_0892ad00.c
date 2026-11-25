/* Starts a timed move: if the first hook succeeds the move is cancelled, otherwise the second hook runs and the rate is set up. */
typedef struct Pmf { short adj; short pad; int (*fn)(void *); } Pmf;
typedef struct Hooks { char p0[0x18]; Pmf a; char p1[8]; Pmf b; } Hooks;
typedef struct S { char p0[0xC]; float f0C; float time; float f14; float rate; char p1[4]; unsigned char active; char p2[3]; Hooks *h; } S;
void func_0892AD00(S *self, float t)
{
    Pmf *a = &self->h->a;
    if (a->fn((char *)self + a->adj)) {
        self->active = 0;
    } else {
        Pmf *b = &self->h->b;
        b->fn((char *)self + b->adj);
        self->time = t;
        self->active = 1;
        self->f0C = 0.0f;
        self->f14 = 0.0f;
        self->rate = 1.0f / t;
    }
}
