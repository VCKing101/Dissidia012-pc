typedef struct KeyPair {
    int key;
    int value;
} KeyPair;

typedef struct KeyTable {
    unsigned char active;
    char pad_01[0x3];
    KeyPair entries[20];
} KeyTable;

int func_08A04C44(KeyTable *self, int key)
{
    int result;
    if (!self->active) {
        return 0;
    }
    {
        int i = 0;
        result = 0;
        while (i < 20) {
            KeyPair *pair = &self->entries[i];
            if (pair->key != key) {
                i++;
                continue;
            }
            return pair->value;
        }
    }
    return result;
}
