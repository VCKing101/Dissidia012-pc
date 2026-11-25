/* Starts a 12-step animation on the owner's block using the id read from the argument, then sets state bits. */
typedef struct R39644 { int a; int flags; } R39644;
typedef struct B39644 { int h; char pad[0x1C]; void *t; } B39644;
typedef struct S39644 { char pad0[0x44]; unsigned flags; char pad1[0xC4 - 0x48]; R39644 *r; B39644 *b; } S39644;
extern int func_088729A0(int);
extern void func_0892E2C4(void *, int, int, int, int, int, int, float);
extern void func_0892E318(void *);
void func_08B39644(S39644 *self, int *id)
{
    int i = *id;
    void *t = self->b->t;
    int h = self->b->h;
    func_0892E2C4(t, h, 0xC, i, 0, 0, func_088729A0(0), 0.0f);
    func_0892E318(self->b->t);
    self->flags |= 8;
    self->r->flags |= 2;
}
