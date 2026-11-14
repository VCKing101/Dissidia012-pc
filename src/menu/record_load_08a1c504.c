/* Loads saved record `index` into the page: its 0x18-byte header and the whole record. */
typedef struct SavedRecord {
    int field_00;
    char pad_0004[0x38 - 0x4];
    char field_38[0x18];
    char pad_0050[0x1270 - 0x50];
} SavedRecord;

typedef struct SavedRecords {
    char pad_00000[0x14C78];
    SavedRecord field_14C78[1];
} SavedRecords;

typedef struct RecordPage {
    char pad_00000[0x6964];
    SavedRecords *field_6964;
    char pad_06968[0x7BE0 - 0x6968];
    SavedRecord field_7BE0;
    char pad_08E50[0x8E60 - 0x8E50];
    unsigned int field_8E60;
    char pad_08E64[0x25B64 - 0x8E64];
    char field_25B64[0x18];
    short field_25B7C;
} RecordPage;

typedef struct RecordOwner {
    char pad_000[0x154];
    RecordPage *field_154;
} RecordOwner;

extern void func_08935524(void *dst, const void *src, int size);

void func_08A1C504(RecordOwner *self, int index)
{
    SavedRecord *record = &self->field_154->field_6964->field_14C78[index];
    func_08935524(self->field_154->field_25B64, record->field_38, 0x18);
    self->field_154->field_25B7C = 0;
    self->field_154->field_7BE0 = *record;
    self->field_154->field_8E60 = self->field_154->field_8E60 | 0x1000;
}
