typedef struct Counts08A3C164 {
    char pad_00[6];
    unsigned short count;
} Counts08A3C164;

typedef struct Row08A3C164 {
    char data[76];
} Row08A3C164;

typedef struct Tables08A3C164 {
    char pad_00[4];
    Row08A3C164 *rows;
} Tables08A3C164;

extern Tables08A3C164 D_09B6DA98;
extern Counts08A3C164 *D_08A3CCF0;

Row08A3C164 *func_08A3C164(int index)
{
    Row08A3C164 *rows = D_09B6DA98.rows;
    if (rows != 0) {
        if (index < 0 || (int)(D_08A3CCF0->count - 1) < index) {
            index = 0;
        }
        return &rows[index];
    }
    return 0;
}
