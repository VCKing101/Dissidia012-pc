typedef struct RateEntry {
    int id;
    float rate;
} RateEntry;

typedef struct RateList {
    unsigned char active;
    char pad_001[0x15B];
    int count;
    RateEntry *entries;
} RateList;

float func_08AACA88(RateList *self, int id)
{
    if (self->active == 0) {
        return 0.0f;
    }
    {
        int i;
        for (i = 0; i < self->count; i++) {
            RateEntry *entry = &self->entries[i];
            if (entry->id == id) {
                return entry->rate;
            }
        }
    }
    return 0.0f;
}
