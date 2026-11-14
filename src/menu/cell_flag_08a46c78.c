typedef struct CellGrid {
    char pad_000[0x410];
    int field_410;
} CellGrid;

typedef struct CellOwner {
    char pad_000[0x418];
    CellGrid *field_418;
} CellOwner;

extern int func_08A48450(int grid, int column, int row);
extern unsigned int func_08A484D8(int grid, int layer, int cell);

int func_08A46C78(CellOwner *self, int column, int offset, int row)
{
    int cell[2]; /* two-word cell reference; only the index is used here */
    cell[0] = func_08A48450(self->field_418->field_410, column + offset, row);
    if (cell[0] == -1) {
        return 0;
    }
    if ((func_08A484D8(self->field_418->field_410, 0, cell[0]) & 4) != 0) {
        return 1;
    }
    return 0;
}
