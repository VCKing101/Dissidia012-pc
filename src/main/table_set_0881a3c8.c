/* Stores a byte value at index: -1 when the table is invalid, -2 out of range. */
typedef struct Table0881A3C8 {
    unsigned char valid;
    char pad_01[3];
    unsigned int count;
    unsigned int *values;
} Table0881A3C8;

int func_0881A3C8(Table0881A3C8 *t, unsigned int index, unsigned char value)
{
    if (!t->valid) {
        return -1;
    }
    if (index >= t->count) {
        return -2;
    }
    t->values[index] = value;
    return 0;
}
