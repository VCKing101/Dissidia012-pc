typedef struct Calendar {
    char pad_00[4];
    unsigned char today;
    unsigned char changed;
} Calendar;

typedef struct SaveData {
    char pad_00000[0x1DE84];
    Calendar calendar;
} SaveData;

extern SaveData *D_089D712C;

void func_088F0C98(int today)
{
    Calendar *cal = &D_089D712C->calendar;

    cal->today = today;
    cal->changed = 0;
}
