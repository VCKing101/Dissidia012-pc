/* Looks up an entry by name pair; if found stores func_08B52F88 of its fields 4 and 8 at +0x10. */
struct Ent { int a; int b; int c; };
struct Obj { char pad[0x10]; int v; };
extern struct Ent *func_08869AB4(void *, void *);
extern int func_08B52F88(int, int);
void func_08B61E50(struct Obj *self, void *a, void *b)
{
    struct Ent *e = func_08869AB4(a, b);
    if (e == 0) {
        return;
    }
    self->v = func_08B52F88(e->b, e->c);
}
