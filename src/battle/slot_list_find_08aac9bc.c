typedef struct SlotEntry {
    int id;
    char pad_04[0x20];
} SlotEntry;

typedef struct SlotList {
    int count;
    SlotEntry *entries;
} SlotList;

typedef struct SlotOwner {
    unsigned char active;
    char pad_01[0xE3];
    SlotList slots;
} SlotOwner;

SlotEntry *func_08AAC9BC(SlotOwner *self, int id)
{
    SlotList *slots;

    if (self->active == 0) {
        return 0;
    }
    slots = &self->slots;
    {
        int i;
        for (i = 0; i < slots->count; i++) {
            SlotEntry *entry = &slots->entries[i];
            if (entry->id == id) {
                return entry;
            }
        }
    }
    return 0;
}
