/* Allocates and constructs a 0x18-byte pool on `heap` (new (heap) Pool(heap)).
   Retail keeps both the heap and the new-expression's null result in the
   frame (4($sp) and 0($sp)) across the allocation call. One-element arrays
   are never promoted by SNC -O2, so they reproduce those slots; `one` and
   `none` are the named temporaries the inlined operator new leaves (ori). */
struct Pool0888232C {
    void *heap;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
};

extern "C" {

void *func_0885C6CC(void *heap, unsigned size, int align, int offset, int kind, int flags);

Pool0888232C *func_0888232C(void *heap)
{
    Pool0888232C *result[1];
    void *owner[1];
    owner[0] = heap;
    int one = 1;
    Pool0888232C *none = 0;
    result[0] = none;
    Pool0888232C *self = (Pool0888232C *)func_0885C6CC(heap, sizeof(Pool0888232C), 0, 0, one, 0);
    Pool0888232C *made = result[0];
    void *h = owner[0];
    if (self != 0) {
        self->heap = h;
        self->f8 = 0;
        self->f4 = 0;
        self->fC = 0;
        self->f10 = 0;
        self->f14 = 0;
        made = self;
    }
    return made;
}

}
