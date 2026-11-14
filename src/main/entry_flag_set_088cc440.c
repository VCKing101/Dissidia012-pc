/* Sets the flag bit of entry i (0 when out of range) unless it has none. */
typedef struct Header088CC440 {
    short field_00;
    unsigned short count;
} Header088CC440;

typedef struct Entry088CC440 {
    int a;
    unsigned short flag;
    short field_06;
} Entry088CC440;

extern Header088CC440 *D_089D7808;
extern Entry088CC440 *D_089D780C;
extern unsigned int *D_089D726C;
void func_088CC440(int i)
{
    if (D_089D7808 != 0) {
        unsigned int bit;
        if (i < 0 || i >= D_089D7808->count) {
            i = 0;
        }
        bit = D_089D780C[i].flag;
        if (bit != 0xFFFF) {
            D_089D726C[bit >> 5] |= 1 << (bit & 0x1F);
        }
    }
}
