typedef struct DateTime { unsigned short year; unsigned short month; unsigned short day; unsigned short hour; unsigned short minute; unsigned short second; unsigned int microsecond; } DateTime;
typedef struct SaveDate { unsigned short year; unsigned char month; unsigned char day; } SaveDate;
typedef struct Save { char data_00000[0x1DF84]; SaveDate date; } Save;
extern Save *D_089D712C;
extern void func_089C5D98(DateTime *d);

void func_088F0D50(void)
{
    DateTime d;
    SaveDate *date;
    func_089C5D98(&d);
    date = &D_089D712C->date;
    date->year = d.year;
    date->month = d.month;
    date->day = d.day;
}
