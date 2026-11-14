typedef struct ValueTable {
    unsigned char active;
    char pad_001[0x27F];
    unsigned int count;
    int *values;
} ValueTable;

int func_08AAD1E4(ValueTable *self, unsigned int index)
{
    if (self->active == 0) {
        return -1;
    }
    if (index >= self->count) {
        return -1;
    }
    return self->values[index];
}
