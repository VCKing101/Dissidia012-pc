typedef struct PoolReset65A84 {
    unsigned char active;
    char pad_001[0x323];
    int count;
    int field_328;
} PoolReset65A84;

int func_08A65A84(PoolReset65A84 *self)
{
    if (!self->active) {
        return -1;
    }
    self->count = 0;
    self->field_328 = 0;
    return 0;
}
