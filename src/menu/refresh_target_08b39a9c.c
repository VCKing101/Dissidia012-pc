/* Refreshes the focused target: resets the effect state, re-selects from the owner's list and, when a pointer is attached, moves it to the target's position. */
typedef struct V3 { float x, y, z; } V3;
typedef struct A39A9C { char pad[0x24]; int e; } A39A9C;
typedef struct C39A9C { char pad0[0x10]; int list; char pad1[0x2C - 0x14]; void *ptr; } C39A9C;
typedef struct S39A9C { char pad0[0xC4]; C39A9C *c; A39A9C *a; } S39A9C;
extern void func_08B4E794(int);
extern void func_08B43E78(void *);
extern int func_08B46DB8(int);
extern void func_08B3C3FC(S39A9C *, int);
extern int func_08B46DC4(int);
extern void func_08B408E0(S39A9C *, int, int);
extern void func_08923018(V3 *, int);
extern void func_08B43E68(void *);
extern void func_08B43F00(void *, V3 *);
void func_08B39A9C(S39A9C *self)
{
    C39A9C *c;
    int list;
    int t;
    func_08B4E794(self->a->e);
    c = self->c;
    if (c->ptr) {
        func_08B43E78(c->ptr);
        c = self->c;
    }
    list = c->list;
    func_08B3C3FC(self, func_08B46DB8(list));
    t = func_08B46DC4(list);
    if (t) {
        func_08B408E0(self, t, 0);
        if (self->c->ptr) {
            V3 v;
            V3 r;
            func_08923018(&r, t);
            v = r;
            func_08B43E68(self->c->ptr);
            func_08B43F00(self->c->ptr, &v);
        }
    }
}
