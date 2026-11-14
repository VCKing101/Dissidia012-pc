typedef struct RecordData {
    int words[0x55];
} RecordData;

typedef struct RecordState {
    char pad_0000[0x79A4];
    int field_79A4;
    char pad_79A8[0x7A6C - 0x79A8];
    RecordData field_7A6C;
} RecordState;

typedef struct RecordOwner {
    char pad_00[0x58];
    RecordState *field_58;
} RecordOwner;

void func_08A511B0(RecordOwner *self, RecordData *record)
{
    self->field_58->field_79A4 = 2;
    self->field_58->field_7A6C = *record;
}
