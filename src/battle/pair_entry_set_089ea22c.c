typedef struct PairEntry84C {
    char data[0x84C];
} PairEntry84C;

typedef struct PairTable {
    char pad_00[0x1C];
    PairEntry84C entries[2];
} PairTable;

extern int func_089EA11C(PairTable *self);
extern int func_089EAB30(PairEntry84C *entry, int value);

int func_089EA22C(PairTable *self, unsigned int index, int value)
{
    if (!func_089EA11C(self)) {
        return -1;
    }
    if (index >= 2 || value < 0) {
        return -2;
    }
    if (func_089EAB30(&self->entries[index], value) != 0) {
        return -3;
    }
    return 0;
}
