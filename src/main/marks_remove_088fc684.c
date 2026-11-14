/* Clears `mask` from the mark with `id`. */
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

void func_088FC684(MarkOwner *owner, short id, unsigned short mask)
{
    int i;

    for (i = 0; i < 50; i++) {
        if (owner->table->marks[i].id == id) {
            owner->table->marks[i].mask &= ~mask;
            break;
        }
    }
}
