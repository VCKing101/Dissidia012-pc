/* Counts the low four flag bits of day `index`. */
typedef struct SaveData {
    char pad_00000[0x1DE90];
    unsigned char day_flags[7];
} SaveData;

extern SaveData *D_089D712C;

int func_088F0438(int index)
{
    unsigned char *day = (unsigned char *)D_089D712C + index;
    int offset = 0x1DE90;
    unsigned char bits = day[offset];
    int count = 0;

    if (bits & 1) {
        count++;
    }
    if (bits & 2) {
        count++;
    }
    if (bits & 4) {
        count++;
    }
    if (bits & 8) {
        count++;
    }
    return count;
}
