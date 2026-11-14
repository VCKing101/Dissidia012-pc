typedef struct Counts08A3C210 {
    char pad_00[14];
    unsigned short count;
} Counts08A3C210;

typedef struct Row08A3C210 {
    char data[28];
} Row08A3C210;

typedef struct Tables08A3C210 {
    char pad_00[20];
    Row08A3C210 *rows;
} Tables08A3C210;

extern Tables08A3C210 D_09B6DA98;
extern Counts08A3C210 *D_08A3CCF0;

Row08A3C210 *func_08A3C210(int index)
{
    Row08A3C210 *rows = D_09B6DA98.rows;
    if (rows != 0) {
        if (index < 0 || (int)(D_08A3CCF0->count - 1) < index) {
            index = 0;
        }
        return &rows[index];
    }
    return 0;
}
