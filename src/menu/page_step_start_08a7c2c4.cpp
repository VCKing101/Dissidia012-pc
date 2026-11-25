/* Menu page step: state 0 starts the page parts and moves to state 1; state 1 waits for the accept masks, then resets the parts and the shared word. */
class Mgr {
public:
    char pad_00[0x34];
    virtual void v0();
    virtual int v1(unsigned long long mask);
};
struct In { char p[0x34]; char f34[4]; };
struct Other { char p[0x148]; In in; };
struct St { int state; int arg; };
struct Page {
    char p0[4];
    char f04[0x42C];
    char f430[0x10];
    char f440[0x9B60 - 0x440];
    int f9B60;
    char p1[0x0C];
};
extern "C" {
extern unsigned long long D_08A7CA08, D_08A7CA10;
Mgr *func_088729A0(int index);
void func_08A743E0(void *);
void func_08A744BC(void *, void *, int, int, int, int);
void func_08A72F5C(void *);
void func_08A75A1C(void *);
void func_08A77614(Page *, int);
void func_08A7C2C4(Page *self, Other *other)
{
    St *st = (St *)((char *)self + 0x9B6C);
    switch (st->state) {
    case 0: {
        In *in = &other->in;
        char *f = in->f34;
        func_08A744BC(self->f430, f, 0, st->arg, 0, 0);
        func_08A72F5C(self->f440);
        st->state = 1;
        break;
    }
    case 1: {
        Mgr *m = func_088729A0(0);
        if (m->v1(D_08A7CA08) != 0 || m->v1(D_08A7CA10) != 0) {
            func_08A743E0(self->f430);
            func_08A75A1C(self->f04);
            self->f9B60 = 0;
            func_08A77614(self, 2);
        }
    }
    default:
        break;
    }
}
}
