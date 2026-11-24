/* Validates the kind (< 0x21), builds a six-word record on the stack and submits it to the list at +0x64; -1..-4 on failure. */
typedef struct SubmitSelf { char pad[4]; int state; char pad_08[0x60]; void *list; } SubmitSelf;
typedef struct Rec { int a; unsigned kind; int c; int d; int e; int f; } Rec;
extern int func_08B1D274(void);
extern int func_08B1CA38(void *list, Rec *r);
int func_08B1D464(SubmitSelf *self, int a, unsigned kind, int c, int d, int e, int f)
{
    Rec r;
    void *list;
    SubmitSelf *me = self;
    int aa = a;
    if (func_08B1D274() == 0) {
        return -1;
    }
    if (kind >= 0x21) {
        return -2;
    }
    if (me->state != 1) {
        return -3;
    }
    r.a = aa;
    r.kind = kind;
    r.c = c;
    r.d = d;
    r.e = e;
    list = me->list;
    r.f = f;
    if (func_08B1CA38(list, &r) != 0) {
        return -4;
    }
    return 0;
}
