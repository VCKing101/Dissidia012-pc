// Releases entry idx: finishes its object (virtual 4), frees its handle and clears the three table cells.
extern "C" void func_08942998(struct Obj *o, int cfg);
struct Obj {
    int state;
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void target(int a);
};
struct Pool {
    Obj **objs; int p1; int *arr8; int *arr3; int *arr4; int p5; int p6; int p7; void (*release)(int); int p9; int cfg;
    int Cfg() { return cfg; }
    static void head(Pool *self, int idx)
    {
        int off = idx * 4;
        Obj *first = *(Obj **)((char *)self->objs + off);
        func_08942998(first, self->Cfg());
        Obj *o = *(Obj **)((char *)self->objs + off);
        if (o->state != 0) {
            o->target(0);
        }
    }
    static void tail(Pool *self, int idx)
    {
        int off = idx * 4;
        if (*(int *)((char *)self->arr3 + off) != 0) {
            self->release(*(int *)((char *)self->arr3 + off));
            *(int *)((char *)self->arr3 + off) = 0;
        }
        *(int *)((char *)self->arr8 + off) = 0;
        *(int *)((char *)self->arr4 + off) = 0;
    }
};
extern "C" void func_08942F00(Pool *self, int idx)
{
    Pool::head(self, idx);
    Pool::tail(self, idx);
}
