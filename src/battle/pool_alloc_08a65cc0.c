typedef struct PoolEntry65CC0 {
    int field_00;
    int field_04;
} PoolEntry65CC0;

typedef struct Pool65CC0 {
    unsigned char active;
    char pad_001[0x3];
    PoolEntry65CC0 entries[100];
    int count;
} Pool65CC0;

PoolEntry65CC0 *func_08A65CC0(Pool65CC0 *self)
{
    PoolEntry65CC0 *entry;
    if (self->count >= 100) {
        return 0;
    }
    entry = &self->entries[self->count];
    self->count++;
    return entry;
}
