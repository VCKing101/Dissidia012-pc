/* Copies a 0x1548-byte cell into column `col` of row `row` of the global grid. */
typedef struct GridCell {
    int words[0x1548 / 4];
} GridCell;

typedef struct GridRow {
    char pad_0000[0x70C];
    GridCell cells[4];
    char pad_5C2C[0x6A68 - 0x5C2C];
} GridRow;

typedef struct Grid {
    GridRow rows[1];
} Grid;

extern Grid *D_089D7F38;

void func_088F6F48(int row, int col, GridCell *cell)
{
    D_089D7F38->rows[row].cells[col] = *cell;
}
