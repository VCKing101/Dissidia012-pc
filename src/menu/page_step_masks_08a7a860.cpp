/* Menu page step: waits for the accept masks in state 0, then starts the page parts and moves to state 1; state 1 queries the part and plays message 0xA. */
class Mgr {
public:
    char pad_00[0x34];
    virtual void v0();
    virtual int v1(unsigned long long mask);
};
struct Other { char p[0x1B0]; char f1B0[4]; };
struct Page { char p0[4]; char parts[0x10]; int state; };
extern "C" {
extern unsigned long long D_08A7AA50, D_08A7AA58;
Mgr *func_088729A0(int index);
void func_08A743E0(void *);
void func_08A77614(Page *, int);
int func_08A74564(void *);
void func_08A79E68(void *, int);
void func_08A74414(void *);
void func_08A7A860(Page *self, Other *other)
{
    char *parts = self->parts;
    switch (self->state) {
    case 0: {
        Mgr *m = func_088729A0(0);
        if (m->v1(D_08A7AA50) != 0 || m->v1(D_08A7AA58) != 0) {
            func_08A743E0(parts);
            func_08A77614(self, 2);
            self->state = 1;
        }
        break;
    }
    case 1:
        if (func_08A74564(parts) == 1) {
            func_08A79E68(&other->f1B0, 0xA);
        }
        break;
    }
    func_08A74414(parts);
}
}
