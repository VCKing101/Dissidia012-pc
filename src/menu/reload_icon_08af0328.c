/* Frees the icon handle, clears the owner's three counters, creates the icon from a formatted name and attaches it. */
extern void func_0887BA5C(void *h);
extern void func_089347E8(char *buf, const char *fmt, ...);
extern void *func_0887D868(void *g, int a, void *name, unsigned b, int c, int d, int e, int f);
extern void func_0887BA04(void *h);
extern void func_0887B944(void *h);
extern void func_088D40BC(void *o, void *p, int a, int b, int c, int d);
extern const char D_08AF1208[];
extern const char D_08AF120C[];
extern void *D_089D6998;

typedef struct P0328 { void *h; char pad[0x60]; int id; } P0328;
typedef struct C0328 { char pad[0x30]; int c30; int c34; int c38; } C0328;
typedef struct O0328 { char pad[0x160]; C0328 *cnt; } O0328;
typedef struct S0328 { char pad[0x70]; P0328 *p; } S0328;

void func_08AF0328(S0328 *self, O0328 *o)
{
    char buf[32];
    int id;
    C0328 *c;
    if (self->p->h != 0) {
        func_0887BA5C(self->p->h);
        self->p->h = 0;
    }
    c = o->cnt;
    c->c34 = 0;
    c->c30 = 0;
    c->c38 = 0;
    id = self->p->id;
    if (id == -1) {
        func_089347E8(buf, D_08AF1208);
    } else {
        func_089347E8(buf, D_08AF120C, id);
    }
    self->p->h = func_0887D868(D_089D6998, 0, buf, -1, 0, 0, 0, 0);
    func_0887BA04(self->p->h);
    func_0887B944(self->p->h);
    func_088D40BC(o, self->p, 0, 0x24, 0x58, 1);
}
