/* True when the saved date is today's date. */
typedef struct SaveDate {
    unsigned short year;
    unsigned char month;
    unsigned char day;
} SaveDate;

typedef struct SaveData {
    char pad_00000[0x1DF84];
    SaveDate date;
} SaveData;

typedef struct ClockTime {
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
    unsigned int micro;
} ClockTime;

extern SaveData *D_089D712C;
extern int func_089C5D98(ClockTime *now);

int func_088F0D98(void)
{
    ClockTime now;
    SaveDate *date;

    func_089C5D98(&now);
    date = &D_089D712C->date;
    if (date->year == now.year && date->month == now.month && date->day == now.day) {
        return 1;
    }
    return 0;
}
