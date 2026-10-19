/* Reviewed signed-halfword records; table lengths/semantic columns unknown.
 * The existing byte-array declaration is retained for compatible linkage. */
extern unsigned char D_08A93E24[];
typedef struct MenuHalfwordPair {
    signed short first;
    signed short second;
} MenuHalfwordPair;
extern MenuHalfwordPair D_08A9426C[];
typedef struct MenuHalfwordRecord {
    signed short field_0000;
    signed short field_0002;
    signed short field_0004;
    signed short field_0006;
} MenuHalfwordRecord;

int func_08A93D84(unsigned int index, int second) {
    if (!second) {
        return D_08A9426C[index].first;
    }
    return D_08A9426C[index].second;
}

int func_08A93DAC(unsigned int index) {
    return ((MenuHalfwordRecord *)(index * sizeof(MenuHalfwordRecord) + D_08A93E24))->field_0004;
}
