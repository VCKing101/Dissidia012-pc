typedef struct Counts08A3C2B8 {
    char pad_00[18];
    unsigned short count;
} Counts08A3C2B8;

typedef struct Row08A3C2B8 {
    char data[56];
} Row08A3C2B8;

typedef struct Tables08A3C2B8 {
    char pad_00[28];
    Row08A3C2B8 *rows;
} Tables08A3C2B8;

extern Tables08A3C2B8 D_09B6DA98;
extern Counts08A3C2B8 *D_08A3CCF0;

Row08A3C2B8 *func_08A3C2B8(int index)
{
    Row08A3C2B8 *rows = D_09B6DA98.rows;
    if (rows != 0) {
        if (index < 0 || (int)(D_08A3CCF0->count - 1) < index) {
            index = 0;
        }
        return &rows[index];
    }
    return 0;
}
