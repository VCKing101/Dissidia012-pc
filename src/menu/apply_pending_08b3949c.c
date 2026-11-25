/* Applies a pending amount: adds it to the owner's counter, starts a timed animation, sets state bits and resets the pending record. */
typedef struct R3949C { int a; int flags; } R3949C;
typedef struct B3949C { int h; char pad[0x1C]; void *t; } B3949C;
typedef struct S3949C { char pad0[0x38]; int count; char pad1[0x44 - 0x3C]; unsigned flags; char pad2[0xC4 - 0x48]; R3949C *r; B3949C *b; } S3949C;
typedef struct G3949C { char pad[0x48]; short id; short amt; float f; } G3949C;
extern G3949C D_09B6DAC0;
extern int func_088729A0(int);
extern void func_0892E2C4(void *, int, int, int, int, int, int, float);
extern void func_0892E318(void *);
void func_08B3949C(S3949C *self)
{
    G3949C *g = &D_09B6DAC0;
    if (g->amt != 0) {
        int n = g->amt;
        void *t;
        int h;
        float zero;
        int snd;
        self->count += n;
        t = self->b->t;
        h = self->b->h;
        snd = func_088729A0(0);
        zero = 0.0f;
        func_0892E2C4(t, h, 0xC, 8, n, 1, snd, zero);
        func_0892E318(self->b->t);
        self->flags |= 8;
        self->r->flags |= 2;
        g->amt = 0;
        g->id = -1;
        g->f = zero;
    }
}
