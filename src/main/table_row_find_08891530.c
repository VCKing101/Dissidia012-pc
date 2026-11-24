/* Reads the first row of a table entry into `out`, following a kind-6 link
   row to its target; returns 0 or a negative error code. */
typedef struct TableHeader {
    char pad_00[0xC];
    unsigned short first;
    unsigned short count;
    char pad_10[0x58 - 0x10];
} TableHeader;

typedef struct TableRow {
    int words[11];
    short kind;
    short size;
    int tail[4];
} TableRow;

typedef struct Table {
    unsigned char open;
} Table;

extern int func_08891C18(Table *self, TableHeader *header, int id);
extern int func_08891894(Table *self, TableRow *row, int index);
extern int func_088914F0(Table *self);

int func_08891530(Table *self, TableRow *out, int id)
{
    TableHeader header;
    TableRow row;

    if (self->open == 0) {
        return -1;
    }
    if (out == 0) {
        return -2;
    }
    if (func_08891C18(self, &header, id) != 0) {
        return -3;
    }
    if (func_08891894(self, &row, header.first) != 0) {
        return -4;
    }
    if (row.kind == 6) {
        if (func_08891894(self, out, header.first + func_088914F0(self)) != 0) {
            return -5;
        }
    } else {
        *out = row;
    }
    return 0;
}
