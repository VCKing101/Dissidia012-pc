/* Detaches the linked object at +0x28, then releases the owner's handle.
   Retail keeps `self` in the frame (0($sp)) across the three calls: a local
   object with a constructor (Kept) is never promoted by SNC -O2. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Linked0883EC44 {
    char unknown_00[0x20];
    void *handle;
    int unknown_24;
    void *link;
};

extern "C" {

extern void *D_089D6D78;
void func_088A9B18(void *link);
void func_088A5604(void *registry, void *link);
void func_088A522C(void *handle);

void func_0883EC44(Linked0883EC44 *self)
{
    void *link = self->link;
    if (link != 0) {
        Kept<Linked0883EC44 *> saved(self);
        func_088A9B18(link);
        func_088A5604(D_089D6D78, saved.value->link);
        Linked0883EC44 *s = saved.value;
        s->link = 0;
        func_088A522C(s->handle);
    }
}

}
