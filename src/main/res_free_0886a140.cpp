/* Frees the resource at +0x10C from the shared heap and clears it.
   Retail keeps `self` in the frame (0($sp)) across the call: a local object
   with a constructor (Kept) is never promoted by SNC -O2. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Owner0886A140 {
    char unknown_000[0x10C];
    void *resource;
};

struct Heaps0886A140 {
    int unknown_00;
    void *heap;
};

extern "C" {

extern Heaps0886A140 *D_089D5F94;
void func_0885C538(void *heap, void *data, int flags);

void func_0886A140(Owner0886A140 *self)
{
    void *resource = self->resource;
    if (resource != 0) {
        Kept<Owner0886A140 *> saved(self);
        func_0885C538(D_089D5F94->heap, resource, 0);
        saved.value->resource = 0;
    }
}

}
