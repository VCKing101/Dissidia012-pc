/* Value i (0..3, otherwise 0) of the loaded table at 0x089D7338; 0 when none is loaded. */
typedef struct Table088B696C {
    int field_00;
    unsigned short values[4];
} Table088B696C;

extern Table088B696C *D_089D7338;

unsigned short func_088B696C(int i)
{
    if (D_089D7338 != 0) {
        if (i < 0 || i >= 4) {
            i = 0;
        }
        return D_089D7338->values[i];
    }
    return 0;
}
