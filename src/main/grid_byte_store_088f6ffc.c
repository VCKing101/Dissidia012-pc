/* Stores byte (row, col) of the 5-column table and refreshes it. */
typedef struct ByteRow {
    unsigned char cols[5];
} ByteRow;

extern ByteRow *D_089D7F40;
extern void func_088F715C(void);

void func_088F6FFC(int row, int col, int value)
{
    D_089D7F40[row].cols[col] = value;
    func_088F715C();
}
