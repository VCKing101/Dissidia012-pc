/* Updates cell (row, col) of the 0x60-byte cell grid at 0x1A8. */
typedef struct GridCell60 {
    char data[0x60];
} GridCell60;

typedef struct GridRow60 {
    GridCell60 cells[0xD20 / 0x60];
} GridRow60;

typedef struct CellGrid {
    char pad_000[0x1A8];
    GridRow60 rows[4];
} CellGrid;

extern void func_08902B38(GridCell60 *cell, int arg);

void func_089029D4(CellGrid *grid, int arg, int row, int col)
{
    GridRow60 *rows = grid->rows;

    func_08902B38(&rows[row].cells[col], arg);
}
