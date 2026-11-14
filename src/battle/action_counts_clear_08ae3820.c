typedef struct ActionCounts {
    int pad_00;
    int field_04;
    char pad_08[0x8];
    int field_10;
    int field_14;
    int pad_18;
    int field_1C;
} ActionCounts;

typedef struct ActionRecord {
    char pad_00[0x8];
    ActionCounts counts;
} ActionRecord;

/* 1 when the record's four counters are all zero. */
int func_08AE3820(void *self, ActionRecord *record)
{
    ActionCounts *counts = &record->counts;
    if (counts->field_04 == 0 && counts->field_10 == 0 && counts->field_14 == 0 && counts->field_1C == 0) {
        return 1;
    }
    return 0;
}
