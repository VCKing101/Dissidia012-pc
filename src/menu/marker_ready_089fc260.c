typedef struct MenuMarkerTable {
    char pad_0000[0x2E84];
    int field_2E84[5];
    unsigned int field_2E98[5];
} MenuMarkerTable;

typedef struct MenuMarkerOwner {
    char pad_000[0x14C];
    MenuMarkerTable *field_14C;
} MenuMarkerOwner;

int func_089FC260(MenuMarkerOwner *self) {
    int result = 1;
    int i;
    for (i = 0; i < 5; i++) {
        if (self->field_14C->field_2E84[i] == 0) {
            result = 0;
            break;
        }
    }
    return result;
}

int func_089FC298(MenuMarkerOwner *self) {
    int count = 0;
    int i;
    for (i = 0; i < 5; i++) {
        if (self->field_14C->field_2E84[i] != 0 && (self->field_14C->field_2E98[i] & 1) == 0) {
            count++;
        }
    }
    return count;
}
