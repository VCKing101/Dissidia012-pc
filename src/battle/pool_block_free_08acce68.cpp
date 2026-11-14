/* Free the pool's block, then reset it. Retail keeps self in the frame across the free. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

typedef struct PoolBlock {
    int pad_00;
    void *block;
    void *pool;
} PoolBlock;

extern "C" {

extern void func_0885C538(void *pool, void *block, int flags);
extern void func_08ACCE58(PoolBlock *self);

void func_08ACCE68(PoolBlock *self)
{
    void *block = self->block;
    if (block != 0) {
        void *pool = self->pool;
        if (pool != 0) {
            Kept<PoolBlock *> saved(self);
            func_0885C538(pool, block, 0);
            self = saved.value;
        }
    }
    func_08ACCE58(self);
}

}
