typedef struct Agent2SystemInfo {
    unsigned char unknown_0000[0xC];
    unsigned short field_000C;
} Agent2SystemInfo;

extern Agent2SystemInfo D_09B26AB8;
extern int D_089D5ECC;
extern unsigned char D_089D5F1D;
extern int D_089D5F24;
extern int func_089C5D60(int *, int *);
extern int func_089C5888(int, int *);

void func_08867800(void) {
    int first = 0;
    int second = 0;
    int value;
    if (func_089C5D60(&first, &second) >= 0) {
        Agent2SystemInfo *info = &D_09B26AB8;
        if (info->field_000C & 2) D_089D5ECC = 1;
        if (second == 1) D_089D5F1D = 1;
    }
    if (func_089C5888(4, &value) == 0) D_089D5F24 = value;
}
