/* Detaches the linked object at +8, then releases the handle at +0.
   Same shape as func_0883EC44: `self` stays in the frame (0($sp)) across the
   calls through a local object with a constructor (Kept). */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Linked08895B84 {
    void *handle;
    int unknown_04;
    void *link;
};

extern "C" {

extern void *D_089D6D78;
void func_088A9B18(void *link);
void func_088A5604(void *registry, void *link);
void func_088A522C(void *handle);

void func_08895B84(Linked08895B84 *self)
{
    void *link = self->link;
    if (link != 0) {
        Kept<Linked08895B84 *> saved(self);
        func_088A9B18(link);
        func_088A5604(D_089D6D78, saved.value->link);
        Linked08895B84 *s = saved.value;
        s->link = 0;
        func_088A522C(s->handle);
    }
}

}
