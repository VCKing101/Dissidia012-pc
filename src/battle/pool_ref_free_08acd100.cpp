/* Return the held block to its pool. Retail keeps self in the frame across the free. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

typedef struct PoolRef {
    void *pool;
    void *block;
    unsigned char flag_08;
} PoolRef;

extern "C" {

extern void func_0885C538(void *pool, void *block, int flags);

void func_08ACD100(PoolRef *self)
{
    void *block = self->block;
    if (block != 0) {
        Kept<PoolRef *> saved(self);
        func_0885C538(self->pool, block, 0);
        self = saved.value;
    }
    self->block = 0;
}

}
