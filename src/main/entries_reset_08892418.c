typedef struct Entry08892418 {
    short id;
    short field_02;
    unsigned char field_04;
    unsigned char field_05;
    unsigned char field_06;
    unsigned char unknown_07;
    unsigned char field_08;
    unsigned char unknown_09;
} Entry08892418;

typedef struct Owner08892418 {
    unsigned char unknown_000[0x612];
    Entry08892418 entries[16];
} Owner08892418;

void func_08892418(Owner08892418 *self) {
    int i;
    for (i = 0; i < 16; i++) {
        Entry08892418 *entry = &self->entries[i];
        entry->id = -1;
        entry->field_02 = 0;
        entry->field_04 = 0;
        entry->field_05 = 0;
        entry->field_06 = 0xFF;
        entry->field_08 = 0;
    }
}
