typedef struct DateTime { unsigned short year; unsigned short month; unsigned short day; unsigned short hour; unsigned short minute; unsigned short second; unsigned int microsecond; } DateTime;
extern void func_089C5D98(DateTime *d);
extern void func_088F4D34(void *rec, unsigned short year, unsigned short month, unsigned short day, unsigned short hour, unsigned short minute);

void func_088F4D70(void *rec)
{
    DateTime d;
    func_089C5D98(&d);
    func_088F4D34(rec, d.year, d.month, d.day, d.hour, d.minute);
}
