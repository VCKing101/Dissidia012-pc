typedef struct Counts08A3C110 {
    char pad_00[4];
    unsigned short count;
} Counts08A3C110;

typedef struct Row08A3C110 {
    char data[48];
} Row08A3C110;

typedef struct Tables08A3C110 {
    char pad_00[0];
    Row08A3C110 *rows;
} Tables08A3C110;

extern Tables08A3C110 D_09B6DA98;
extern Counts08A3C110 *D_08A3CCF0;

Row08A3C110 *func_08A3C110(int index)
{
    Row08A3C110 *rows = D_09B6DA98.rows;
    if (rows != 0) {
        if (index < 0 || (int)(D_08A3CCF0->count - 1) < index) {
            index = 0;
        }
        return &rows[index];
    }
    return 0;
}
