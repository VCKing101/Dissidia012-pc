typedef struct ReadySlot {
    unsigned char ready;
    unsigned char busy;
    char pad_02[0xA];
} ReadySlot;

typedef struct ReadyTable {
    unsigned char active;
    char pad_01[0xF];
    ReadySlot slots[34];
} ReadyTable;

unsigned char func_08A826F4(ReadyTable *self, unsigned int index)
{
    ReadySlot *slots;
    ReadySlot *slot;
    int result;
    if (!self->active) {
        return 0;
    }
    if (index >= 34) {
        return 0;
    }
    slots = self->slots;
    slot = &slots[index];
    result = 0;
    if (slot->ready == 0 && slot->busy == 1) {
        result = 1;
    }
    return result;
}
