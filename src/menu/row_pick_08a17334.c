/* Picks the page row: 2 plus the first empty item, or 2 when the entry is not in list mode. */
typedef struct RowItem {
    short field_00;
    char pad_02[0x38 - 0x2];
} RowItem;

typedef struct RowEntry {
    char pad_000[0xD4];
    int field_D4;
    char pad_0D8[0xF8 - 0xD8];
    RowItem field_F8[5];
    char pad_210[0x24C - 0x210];
} RowEntry;

typedef struct RowTable {
    char pad_0000[0x2BC0];
    RowEntry field_2BC0[1];
} RowTable;

typedef struct RowRenderer {
    char pad_000[0x154];
    RowTable *field_154;
} RowRenderer;

typedef struct RowPage {
    char pad_000[0xC7C];
    int field_C7C;
    char pad_C80[0xC94 - 0xC80];
    int field_C94;
} RowPage;

typedef struct RowOwner {
    char pad_000[0xD80];
    RowPage *field_D80;
} RowOwner;

extern RowRenderer *D_08A23FF0;

void func_08A17334(RowOwner *self)
{
    RowPage *page = self->field_D80;
    RowEntry *entry = &D_08A23FF0->field_154->field_2BC0[page->field_C7C];
    if (entry->field_D4 == 1) {
        int row = 0;
        int i;
        for (i = 0; i < 5; i++) {
            if (entry->field_F8[i].field_00 == 0) {
                break;
            }
            row++;
        }
        page->field_C94 = row + 2;
    } else {
        page->field_C94 = 2;
    }
}
