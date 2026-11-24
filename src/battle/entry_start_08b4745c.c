// Starts an entry: stores its id, passes a copy of the colour vector on to the setup calls, and marks it active.
typedef struct V4 { float x, y, z, w; } V4;
struct Self { int active; int id; };
extern void func_08B47640(struct Self *, int, int, int, V4 *);
extern void func_08B47764(struct Self *, int, int, int);
extern void func_08B4758C(struct Self *);

void func_08B4745C(struct Self *self, int a1, int a2, int a3, V4 *color, int b1, int b2, int b3, int id)
{
    struct { int pad[4]; V4 c; } l;
    self->id = id;
    l.c = *color;
    func_08B47640(self, a1, a2, a3, &l.c);
    func_08B47764(self, b1, b2, b3);
    func_08B4758C(self);
    self->active = 1;
}
