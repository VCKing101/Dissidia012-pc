/* Per-cell use counters: rows of five ints from offset 0x258C, the counter is word 3 of the cell. */
typedef struct CountCell {
    int field_00;
    int field_04;
    int field_08;
    int count;
} CountCell;

typedef struct CountRow {
    int cells[5];
} CountRow;

typedef struct CountOwner {
    char pad_00[0x258C];
    CountRow rows[1];
} CountOwner;

typedef struct CountKey {
    char pad_00[0x38];
    unsigned char row;
    unsigned char col;
} CountKey;

void func_0884C474(CountOwner *self, CountKey *key)
{
    CountRow *row = &self->rows[key->row];
    CountCell *cell = (CountCell *)&row->cells[key->col];
    cell->count++;
}

void func_0884C4A8(CountOwner *self, CountKey *key)
{
    CountRow *row = &self->rows[key->row];
    CountCell *cell = (CountCell *)&row->cells[key->col];
    cell->count--;
}
