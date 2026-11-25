/* Advances the rotation counter (byte +0x763, 0..4) of the record found for an id in 1..0x22 (not 0x17) and returns 1, else -1. */
typedef struct Rec08A804B8 { short h[0x3B3]; } Rec08A804B8;
extern char *D_089D712C;
extern char *func_088CB164(char *base, int id);
int func_08A804B8(int self, int id)
{
    int cell[2];
    Rec08A804B8 *rec;
    Rec08A804B8 *dst;
    Rec08A804B8 *src;
    if (id <= 0 || id >= 0x23) {
        return -1;
    }
    if (id == 0x17) {
        return -1;
    }
    rec = (Rec08A804B8 *)(func_088CB164(D_089D712C + 0xF58, id) + 0x10);
    cell[0] = ((char *)rec)[0x763];
    cell[0] = cell[0] + 1;
    if (cell[0] >= 5) {
        cell[0] = 0;
    }
    ((char *)rec)[0x763] = cell[0];
    dst = rec;
    src = rec;
    *dst = *src;
    return 1;
}
