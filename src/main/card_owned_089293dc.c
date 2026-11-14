typedef struct SaveData { char pad_000[0xF58]; char cards[4]; } SaveData;
extern SaveData *D_089D712C;
extern unsigned char func_088CB1EC(void *cards, int id);
int func_089293DC(int id)
{
    if (id <= 0 || id >= 35) {
        return 0;
    }
    if (func_088CB1EC(D_089D712C->cards, id) == 0) {
        return 0;
    }
    return 1;
}
