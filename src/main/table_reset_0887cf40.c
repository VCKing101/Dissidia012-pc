typedef struct Entry0887CF40 {
    unsigned char field_00;
    unsigned char unknown_01[3];
    int field_04;
    int field_08;
    int field_0C;
    unsigned char field_10;
    unsigned char unknown_11[3];
} Entry0887CF40;

typedef struct Table0887CF40 {
    unsigned char field_00;
    unsigned char unknown_01[3];
    Entry0887CF40 entries[10];
} Table0887CF40;

void func_0887CF40(Table0887CF40 *self) {
    int i;
    self->field_00 = 0;
    for (i = 0; i < 10; i++) {
        Entry0887CF40 *entry = &self->entries[i];
        entry->field_00 = 0;
        entry->field_04 = 0;
        entry->field_08 = -1;
        entry->field_0C = 0;
        entry->field_10 = 0;
    }
}
