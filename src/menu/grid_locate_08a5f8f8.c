typedef struct LocateKey {
    unsigned short field_00;
} LocateKey;

typedef struct LocateSource {
    LocateKey *field_00;
} LocateSource;

typedef struct LocateGrid {
    char pad_000[0x328];
    int field_328;
    char pad_32C[0x354 - 0x32C];
    int field_354;
    char pad_358[0x384 - 0x358];
    LocateSource *field_384;
} LocateGrid;

extern int D_08A60030[0x5B];

void func_08A5F8F8(LocateGrid *self)
{
    unsigned short id = self->field_384->field_00->field_00;
    int found;
    int i;
    if (id == 0xFFFF) {
        return;
    }
    found = 0;
    for (i = 0; i < 0x5B; i++) {
        if (id == D_08A60030[i]) {
            found = i;
            break;
        }
    }
    self->field_328 = found / 13;
    self->field_354 = found % 13;
}
