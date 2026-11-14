typedef struct Counts08A3C1C0 {
    char pad_00[12];
    unsigned short count;
} Counts08A3C1C0;

typedef struct Row08A3C1C0 {
    char data[8];
} Row08A3C1C0;

typedef struct Tables08A3C1C0 {
    char pad_00[16];
    Row08A3C1C0 *rows;
} Tables08A3C1C0;

extern Tables08A3C1C0 D_09B6DA98;
extern Counts08A3C1C0 *D_08A3CCF0;

Row08A3C1C0 *func_08A3C1C0(int index)
{
    Row08A3C1C0 *rows = D_09B6DA98.rows;
    if (rows != 0) {
        if (index < 0 || (int)(D_08A3CCF0->count - 1) < index) {
            index = 0;
        }
        return &rows[index];
    }
    return 0;
}
