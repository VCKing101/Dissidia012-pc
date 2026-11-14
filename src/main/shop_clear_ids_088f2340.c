/* Sets the 32 stock ids to -1. */
typedef struct SaveData {
    char pad_00000[0x1DE98];
    short stock[32];
} SaveData;

extern SaveData *D_089D712C;

void func_088F2340(void)
{
    short *stock = D_089D712C->stock;
    int i;

    for (i = 0; i < 32; i++) {
        stock[i] = -1;
    }
}
