typedef struct Agent2SystemInfo {
    unsigned char unknown_0000[0xC];
    unsigned short field_000C;
} Agent2SystemInfo;

extern Agent2SystemInfo D_09B26AB8;
extern int D_089D5ECC;
extern unsigned char D_089D5F1E;
extern int D_089D5ED0[];
extern int D_089D5EF4[];

int func_08867888(void) {
    return D_089D5ECC;
}

int func_08867894(void) {
    return D_089D5ECC == 1;
}
