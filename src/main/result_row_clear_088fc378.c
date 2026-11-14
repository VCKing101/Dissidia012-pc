/* Clears flag 1 of row `index` and drops its mark bit 2. */
typedef struct ResultRow {
    unsigned short flags;
    short id;
    char pad_04[0x1C - 4];
} ResultRow;

typedef struct ResultTable {
    char pad_00[0x1C];
    ResultRow rows[1];
} ResultTable;

typedef struct ResultOwner {
    char pad_00[0x30];
    ResultTable *table;
} ResultOwner;

extern void func_088FC684(ResultOwner *owner, int id, unsigned short mask);

void func_088FC378(ResultOwner *owner, int index)
{
    owner->table->rows[index].flags &= ~2;
    func_088FC684(owner, owner->table->rows[index].id, 2);
}
