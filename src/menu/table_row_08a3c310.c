typedef struct Counts08A3C310 {
    char pad_00[20];
    unsigned short count;
} Counts08A3C310;

typedef struct Row08A3C310 {
    char data[4];
} Row08A3C310;

typedef struct Tables08A3C310 {
    char pad_00[32];
    Row08A3C310 *rows;
} Tables08A3C310;

extern Tables08A3C310 D_09B6DA98;
extern Counts08A3C310 *D_08A3CCF0;

Row08A3C310 *func_08A3C310(int index)
{
    Row08A3C310 *rows = D_09B6DA98.rows;
    if (rows != 0) {
        if (index < 0 || (int)(D_08A3CCF0->count - 1) < index) {
            index = 0;
        }
        return &rows[index];
    }
    return 0;
}
