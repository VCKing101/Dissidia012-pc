/* Empties the fifty (id, mask) marks at 0x592. */
typedef struct Mark {
    short id;
    unsigned short mask;
} Mark;

typedef struct MarkTable {
    char pad_000[0x592];
    Mark marks[50];
} MarkTable;

typedef struct MarkOwner {
    char pad_00[0x30];
    MarkTable *table;
} MarkOwner;

void func_088FC5F8(MarkOwner *owner)
{
    int i;

    for (i = 0; i < 50; i++) {
        owner->table->marks[i].id = -1;
    }
}
