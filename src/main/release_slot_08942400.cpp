// Stops the object in a slot, frees its second table entry through a callback, and clears the slot's entries.
struct O { int active; virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(int a); };
struct T { O **objs; int pad; void **c; void **b; void **d; int p1, p2, p3; void (*freefn)(void *); };
extern "C" void func_08942400(T *self, int i)
{
    O *o = self->objs[i];
    if (o->active != 0)
        o->v4(0);
    if (self->b[i] != 0) {
        self->freefn(self->b[i]);
        self->b[i] = 0;
    }
    self->c[i] = 0;
    self->d[i] = 0;
}
