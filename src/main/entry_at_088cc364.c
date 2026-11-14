/* Entry i (0 when out of range) of the loaded table; 0 when none is loaded. */
typedef struct Header088CC364 {
    short field_00;
    unsigned short count;
} Header088CC364;

typedef struct Entry088CC364 {
    int a;
    int b;
} Entry088CC364;

extern Header088CC364 *D_089D7808;
extern Entry088CC364 *D_089D780C;
Entry088CC364 *func_088CC364(int i)
{
    if (D_089D7808 != 0) {
        if (i < 0 || i >= D_089D7808->count) {
            i = 0;
        }
        return &D_089D780C[i];
    }
    return 0;
}
