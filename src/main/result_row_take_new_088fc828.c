/* Returns the first row with flag 2 set (clearing it), or -1. */
typedef struct ResultRow {
    unsigned short flags;
    short id;
    char pad_04[0x1C - 4];
} ResultRow;

typedef struct ResultTable {
    char pad_00[0x18];
    short count;
    char pad_1A[2];
    ResultRow rows[1];
} ResultTable;

typedef struct ResultOwner {
    char pad_00[0x30];
    ResultTable *table;
} ResultOwner;

int func_088FC828(ResultOwner *owner)
{
    ResultTable *table = owner->table;
    int result = -1;
    int i;

    for (i = 0; i < table->count; i++) {
        if (table->rows[i].flags & 4) {
            table->rows[i].flags &= ~4;
            result = i;
            break;
        }
    }
    return result;
}
