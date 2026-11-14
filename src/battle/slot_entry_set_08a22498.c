typedef struct SlotEntry28 {
    char data[0x28];
} SlotEntry28;

typedef struct SlotOwner22498 {
    unsigned char active;
    char pad_001[0x487];
    SlotEntry28 *entries;
} SlotOwner22498;

extern int func_08890F90(SlotEntry28 *entry, int value);

int func_08A22498(SlotOwner22498 *self, unsigned int index, int value)
{
    if (!self->active) {
        return -1;
    }
    if (index >= 2 || value < 0) {
        return -2;
    }
    if (func_08890F90(&self->entries[index], value) != 0) {
        return -3;
    }
    return 0;
}
