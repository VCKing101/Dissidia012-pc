/* Returns the largest row size in a table as a float, or -1 when the table
   header cannot be read. */
typedef struct TableHeader {
    char pad_00[0xC];
    unsigned short first;
    unsigned short count;
    char pad_10[0x58 - 0x10];
} TableHeader;

typedef struct TableRow {
    char pad_00[0x3A];
    short size;
    char pad_3C[4];
} TableRow;

typedef struct Table Table;

extern int func_088916A4(Table *self, TableHeader *header);
extern int func_08891894(Table *self, TableRow *row, int index);

float func_08891348(Table *self)
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
            size = row.size;
            if (best < size) {
                best = size;
            }
        }
    }
    return best;
}
