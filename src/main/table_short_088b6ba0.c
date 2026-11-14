/* Value i (0..5, otherwise 0) of the loaded table at 0x089D7344; 0 when none is loaded. */
typedef struct Table088B6BA0 {
    unsigned short values[6];
} Table088B6BA0;

extern Table088B6BA0 *D_089D7344;

unsigned short func_088B6BA0(int i)
{
    if (D_089D7344 != 0) {
        if (i < 0 || i >= 6) {
            i = 0;
        }
        return D_089D7344->values[i];
    }
    return 0;
}
