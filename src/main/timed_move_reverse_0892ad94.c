/* Starts a timed move in the other direction: first hook cancels, otherwise the second runs and the rate is -1/t. */
typedef struct Pmf { short adj; short pad; int (*fn)(void *); } Pmf;
typedef struct Hooks { char p0[0x20]; Pmf a; Pmf b; } Hooks;
typedef struct S { char p0[0xC]; float f0C; float time; float f14; float rate; char p1[4]; unsigned char active; char p2[3]; Hooks *h; } S;
void func_0892AD94(S *self, float t)
{
    Pmf *a = &self->h->a;
    if (a->fn((char *)self + a->adj)) {
        self->active = 0;
    } else {
        Pmf *b = &self->h->b;
        b->fn((char *)self + b->adj);
        self->time = t;
        self->f0C = 0.0f;
        self->f14 = 1.0f;
        self->rate = -1.0f / t;
        self->active = 1;
    }
}
