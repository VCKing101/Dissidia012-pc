typedef struct MenuMarker {
    signed char field_00;
    unsigned char field_01;
    char pad_02[2];
} MenuMarker;

typedef struct MenuMarkerTable {
    char pad_0000[0x2E70];
    MenuMarker field_2E70[3];
} MenuMarkerTable;

typedef struct MenuMarkerOwner {
    char pad_000[0x14C];
    MenuMarkerTable *field_14C;
} MenuMarkerOwner;

void func_089F5728(MenuMarkerOwner *self) {
    int i;
    for (i = 0; i < 3; i++) {
        self->field_14C->field_2E70[i].field_00 = -1;
        self->field_14C->field_2E70[i].field_01 = 0;
    }
}
