/* Creates the object's effect instance from its effect resource. */
template <class T> struct Kept0883EBE4 {
    T value;
    Kept0883EBE4(T v) : value(v) {}
};

struct Obj0883EBE4 {
    char pad_00[0x20];
    void *resource;
    int param;
    void *instance;
};

extern "C" {

extern void *D_089D6D78;

int func_088A5208(void *resource);
void *func_088A54B0(void *system, Obj0883EBE4 *owner, int effect, int param, int flags);
void func_088A9A80(void *instance);

void func_0883EBE4(Obj0883EBE4 *self)
{
    if (self->resource != 0) {
        Kept0883EBE4<Obj0883EBE4 *> saved(self);
        int effect = func_088A5208(self->resource);
        void *instance = func_088A54B0(D_089D6D78, saved.value, effect, saved.value->param, 0);
        Obj0883EBE4 *o = saved.value;
        o->instance = instance;
        if (o->instance != 0) {
            func_088A9A80(o->instance);
        }
    }
}

}
