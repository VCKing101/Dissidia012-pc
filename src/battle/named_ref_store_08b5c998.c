/* Looks up a named entry; stores func_08B52F88 of its fields 4 and 8 at +0x48, or 0 when missing. */
struct Ent { int a; int b; int c; };
struct Obj { char pad[0x48]; int v; };
extern const char D_08B5D5BC[];
extern struct Ent *func_08869AB4(void *, const char *);
extern int func_08B52F88(int, int);
void func_08B5C998(struct Obj *self, void *name)
{
    struct Ent *e = func_08869AB4(name, D_08B5D5BC);
    if (e != 0) {
        self->v = func_08B52F88(e->b, e->c);
        return;
    }
    self->v = 0;
}
