/* Looks up two named entries: stores func_08B52F88 of the first at +0x34 (or 0), and passes the second with an extra argument to func_0885AC3C. */
struct Ent { int a; int b; int c; };
struct Obj { char pad[0x34]; int v; };
extern const char D_08B52978[];
extern const char D_08B5298C[];
extern char *D_089D4DC8;
extern struct Ent *func_08869AB4(void *, const char *);
extern int func_08B52F88(int, int);
extern void func_0885AC3C(void *, int, int, int);
void func_08B5220C(struct Obj *self, void *name, int arg)
{
    struct Ent *e = func_08869AB4(name, D_08B52978);
    if (e != 0) {
        self->v = func_08B52F88(e->b, e->c);
        goto next;
    }
    self->v = 0;
next:
    e = func_08869AB4(name, D_08B5298C);
    if (e != 0) {
        func_0885AC3C(D_089D4DC8 + 0x160, arg, e->b, e->c);
    }
}
