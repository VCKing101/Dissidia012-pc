/* Looks up the cell for a row id and a column id in a two-key table. */
struct TableCell {
    int value;
    int extra;
};

struct CellTable {
    unsigned char open;
    unsigned char pad_01[0x17];
    int row_count;
    int *row_ids;
    unsigned char pad_20[4];
    int col_count;
    int *col_ids;
    unsigned char pad_2c[4];
    unsigned int cell_count;
    TableCell *cells;
};

extern "C" {

TableCell *func_089EEEDC(CellTable *table, int primary, int secondary)
{
    if (!table->open) {
        return 0;
    }
    if (primary < 0 || secondary < 0) {
        return 0;
    }
    int row = -1;
    for (int i = 0; i < table->row_count; i++) {
        if (table->row_ids[i] == primary) {
            row = i;
            break;
        }
    }
    int col = -1;
    for (int i = 0; i < table->col_count; i++) {
        if (table->col_ids[i] == secondary) {
            col = i;
            break;
        }
    }
    if ((unsigned int)row >= (unsigned int)table->row_count || (unsigned int)col >= (unsigned int)table->col_count) {
        return 0;
    }
    unsigned int index = row * table->col_count + col;
    if (index >= table->cell_count) {
        return 0;
    }
    return &table->cells[index];
}

}
