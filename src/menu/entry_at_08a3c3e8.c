typedef struct EntryHeader {
    short field_00;
    unsigned short field_02;
} EntryHeader;

typedef struct EntryRow {
    char data[0x2C];
} EntryRow;

extern int D_08A3CCF4;
extern EntryRow *D_08A3CCFC;

EntryRow *func_08A3C3E8(EntryHeader *header, int index)
{
    if (D_08A3CCF4 != 0) {
        if (D_08A3CCFC != 0) {
            if (index < 0 || header->field_02 - 1 < index) {
                index = 0;
            }
            return &D_08A3CCFC[index];
        }
    }
    return 0;
}
