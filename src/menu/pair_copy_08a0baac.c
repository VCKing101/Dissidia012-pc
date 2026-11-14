/* Copies the eleven value pairs into the record and marks it when the source is flagged. */
typedef struct CopyPair {
    short field_00;
    unsigned short field_02;
} CopyPair;

typedef struct CopyPairs {
    CopyPair field_00[11];
} CopyPairs;

typedef struct CopySource {
    char pad_000[0x73C];
    CopyPairs field_73C;
    unsigned char field_768;
} CopySource;

typedef struct CopyRecord {
    char pad_0000[0x2];
    unsigned char field_02;
    char pad_0003[0x27B4 - 0x3];
    CopyPairs field_27B4;
} CopyRecord;

extern void func_0891AC28(CopySource *self, CopyRecord *record);

void func_08A0BAAC(CopySource *self, CopyRecord *record)
{
    CopyPairs *pairs;
    func_0891AC28(self, record);
    record->field_02 = 4;
    pairs = &record->field_27B4;
    *pairs = self->field_73C;
    if (self->field_768 == 1) {
        pairs->field_00[10].field_02 = pairs->field_00[10].field_02 | 1;
    }
}
