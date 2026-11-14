typedef struct PoolRef {
    void *pool;
    void *block;
    unsigned char flag_08;
} PoolRef;

void func_08ACD0F0(PoolRef *self, void *pool)
{
    self->block = 0;
    self->flag_08 = 0;
    self->pool = pool;
}
