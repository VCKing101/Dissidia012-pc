/* Clears the saved date (year, month, day). */
typedef struct SaveDate {
    unsigned short year;
    unsigned char month;
    unsigned char day;
} SaveDate;

typedef struct SaveData {
    char pad_00000[0x1DF84];
    SaveDate date;
} SaveData;

extern SaveData *D_089D712C;

void func_088F0D2C(void)
{
    SaveDate *date = &D_089D712C->date;

    date->year = 0;
    date->month = 0;
    date->day = 0;
}
