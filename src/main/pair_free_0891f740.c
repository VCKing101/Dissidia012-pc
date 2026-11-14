/* Frees the two heap blocks of a pair. Retail keeps `self` at 0($sp) across
   each free: a one-element array (see build/dc-results/stack-spill.md). */
extern void func_0885C538(void *heap, void *data, int flags);
typedef struct BlockPair {
    int field_00;
    void *heap;
    void *first;
    void *second;
} BlockPair;
void func_0891F740(BlockPair *self)
{
    BlockPair *saved[1];
    void *first = self->first;
    if (first != 0) {
        saved[0] = self;
        func_0885C538(self->heap, first, 0);
        self = saved[0];
        self->first = 0;
    }
    {
        void *second = self->second;
        if (second != 0) {
            saved[0] = self;
            func_0885C538(self->heap, second, 0);
            self = saved[0];
            self->second = 0;
        }
    }
}
