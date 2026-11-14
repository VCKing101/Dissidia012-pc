typedef struct UsePair {
    short item;
    short slot;
} UsePair;

typedef struct UseTable {
    char pad_00[0x74];
    unsigned int count;
    UsePair *pairs;
} UseTable;

typedef struct UseOwner {
    unsigned char active;
    char pad_01[0x3];
    UseTable *table;
} UseOwner;

extern int func_08AA7AD0(UseOwner *self, int slot);
extern int func_08899300(void);
extern int func_08897A00(int store, int item);

int func_08AA7BE8(UseOwner *self, unsigned int index)
{
    UsePair *pair;
    if (!self->active) {
        return 0;
    }
    if (index >= self->table->count) {
        return 0;
    }
    pair = &self->table->pairs[index];
    if (func_08AA7AD0(self, pair->slot) == 0) {
        return 0;
    }
    if (func_08897A00(func_08899300(), pair->item) == 0) {
        return 0;
    }
    return 1;
}
