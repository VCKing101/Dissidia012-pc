/* Returns whether func_0884A294 accepts the object's field 0x2FC and the second argument. */
struct Obj { char pad[0x2FC]; int v; };
extern void *D_089D4974;
extern int func_0884A294(void *, struct Obj *, int, int);
int func_08B35218(struct Obj *self, int arg)
{
    int r = func_0884A294(D_089D4974, self, self->v, arg);
    if (r) {
        return 1;
    } else {
        return 0;
    }
}
