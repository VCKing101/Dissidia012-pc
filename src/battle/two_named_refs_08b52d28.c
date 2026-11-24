/* Looks up two named entries: stores func_08B52F88 of the first at +0xC (or 0), and passes the second to func_08854254. */
struct Ent { int a; int b; int c; };
struct Obj { char pad[0xC]; int v; };
extern const char D_08B52EC0[];
extern const char D_08B52ED4[];
extern void *D_089D4DC8;
extern struct Ent *func_08869AB4(void *, const char *);
extern int func_08B52F88(int, int);
extern void func_08854254(void *, int, int);
void func_08B52D28(struct Obj *self, void *name)
{
    struct Ent *e = func_08869AB4(name, D_08B52EC0);
    if (e != 0) {
        self->v = func_08B52F88(e->b, e->c);
        goto next;
    }
    self->v = 0;
next:
    e = func_08869AB4(name, D_08B52ED4);
    if (e != 0) {
        func_08854254(D_089D4DC8, e->b, e->c);
    }
}
