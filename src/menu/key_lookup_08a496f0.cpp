/* Six-byte records after a header; returns the first field of the record whose key matches. */
struct KeyRecord {
    short field_00;
    short field_02;
    short field_04;
};

struct KeyHeader {
    char pad_00[0xA];
    short field_0A;
};

struct KeyTable {
    KeyHeader *field_00;
    int field_04;
    KeyRecord *field_08;
};

extern "C" {
short func_08A496F0(KeyTable *self, int key)
{
    short result = -1;
    for (int i = 0; i < self->field_00->field_0A; i++) {
        KeyRecord *record = &self->field_08[i];
        if (record->field_04 == key) {
            result = record->field_00;
            break;
        }
    }
    return result;
}
}
