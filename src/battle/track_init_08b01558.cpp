typedef struct TrackEntry {
    int id;
    unsigned char field_04;
    unsigned char field_05;
    char pad_06[0x2];
    float field_08;
    float field_0C;
    int field_10;
    float field_14;
} TrackEntry;

typedef struct TrackPair {
    int field_00;
    float field_04;
} TrackPair;

typedef struct TrackSet {
    unsigned char active;
    char pad_01[0x7];
    TrackEntry entries[2];
    int field_38;
    float field_3C;
    int field_40;
    int field_44;
    float field_48;
    int field_4C;
    TrackPair pairs[2];
} TrackSet;

extern "C" {

void func_08B01558(TrackSet *self)
{
    self->active = 0;
    for (int i = 0; i < 2; i++) {
        TrackEntry *entry = &self->entries[i];
        entry->id = -1;
        entry->field_04 = 0;
        entry->field_05 = 0;
        entry->field_08 = 0.0f;
        entry->field_0C = 0.0f;
        entry->field_10 = 0;
        entry->field_14 = 0.0f;
    }
    self->field_38 = -1;
    self->field_3C = 0.0f;
    self->field_40 = 0;
    self->field_44 = -1;
    self->field_48 = 0.0f;
    self->field_4C = 0;
    for (int i = 0; i < 2; i++) {
        TrackPair *pair = &self->pairs[i];
        pair->field_00 = 0;
        pair->field_04 = 0.0f;
    }
}

}
