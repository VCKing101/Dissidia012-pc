/* Returns the largest size among a table's kind-3 rows as a float, or -1
   when the table header cannot be read. */
typedef struct TableHeader {
    char pad_00[0xC];
    unsigned short first;
    unsigned short count;
    char pad_10[0x58 - 0x10];
} TableHeader;

typedef struct TableRow {
    char pad_00[0x2C];
    short kind;
    short size;
    char pad_30[0x40 - 0x30];
} TableRow;

typedef struct Table Table;

extern int func_088916A4(Table *self, TableHeader *header);
extern int func_08891894(Table *self, TableRow *row, int index);

float func_0889140C(Table *self)
{
    TableHeader header;
    float best;

    if (func_088916A4(self, &header) != 0) {
        return -1.0f;
    }
    {
        TableRow row;
        int i;

        best = 0.0f;
        for (i = 0; i < header.count; i++) {
            float size;
            if (func_08891894(self, &row, header.first + i) != 0) {
                continue;
            }
            if (row.kind != 3) {
                continue;
            }
            size = row.size;
            if (best < size) {
                best = size;
            }
        }
    }
    return best;
}
