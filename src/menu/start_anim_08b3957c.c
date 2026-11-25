/* If the linked entry resolves, runs two refresh calls, starts a 12-step animation on the owner's block and sets state bits. */
typedef struct K3957C { char pad[0x10]; void *x; } K3957C;
typedef struct R3957C { int a; int flags; } R3957C;
typedef struct B3957C { int h; char pad[0x1C]; void *t; } B3957C;
typedef struct S3957C { char pad0[0x44]; unsigned flags; char pad1[0xC4 - 0x48]; R3957C *r; B3957C *b; } S3957C;
typedef struct L3957C { char pad[0xC4]; K3957C *k; } L3957C;
extern void *func_08B45EEC(void *);
extern void *func_08B496C0(void *);
extern void func_08B38ED0(void *, void *);
extern void func_08B38EF8(void *);
extern int func_088729A0(int);
extern void func_0892E2C4(void *, int, int, int, int, int, int, float);
extern void func_0892E318(void *);
void func_08B3957C(S3957C *self)
{
    void *e = func_08B496C0(func_08B45EEC(((L3957C *)self)->k->x));
    if (e) {
        void *t;
        int h;
        func_08B38ED0(self, e);
        func_08B38EF8(self);
        t = self->b->t;
        h = self->b->h;
        func_0892E2C4(t, h, 0xC, 0x1D, 0, 0, func_088729A0(0), 20.0f);
        func_0892E318(self->b->t);
        self->flags |= 8;
        self->r->flags |= 2;
    }
}
