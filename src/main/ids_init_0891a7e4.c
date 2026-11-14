typedef struct IdTable {
    short ids[10];
    short values[10];
    short count;                /* 0x28 */
} IdTable;

void func_0891A7E4(IdTable *table)
{
    int i;
    for (i = 0; i < 10; i++) {
        table->ids[i] = -1;
        table->values[i] = 0;
    }
    table->count = 0;
}
