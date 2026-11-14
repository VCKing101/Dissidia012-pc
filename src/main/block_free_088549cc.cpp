/* Frees a heap block if there is one and clears the block fields.
   Retail keeps `self` in the frame (0($sp)) across the call and reloads it
   where both paths join: a Kept local inside the branch, copied back into
   `self` after the call, gives exactly that. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Block088549CC {
    int unknown_00;
    int unknown_04;
    void *data;
    int fC;
    int f10;
    void *heap;
};

extern "C" {

void func_0885C538(void *heap, void *data, int flags);

void func_088549CC(Block088549CC *self)
{
    void *heap = self->heap;
    if (heap != 0) {
        Kept<Block088549CC *> saved(self);
        func_0885C538(heap, self->data, 0);
        self = saved.value;
    }
    self->data = 0;
    self->heap = 0;
    self->f10 = 0;
    self->fC = 0;
}

}
