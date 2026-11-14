typedef struct DateTime { unsigned short year; unsigned short month; unsigned short day; unsigned short hour; unsigned short minute; unsigned short second; unsigned int microsecond; } DateTime;
extern void func_089C5D98(DateTime *d);
extern int func_088ADFA8(unsigned short year, unsigned short month, unsigned short day, unsigned short hour, unsigned short minute);

void func_088ADFCC(int *out)
{
    DateTime d;
    func_089C5D98(&d);
    *out = func_088ADFA8(d.year, d.month, d.day, d.hour, d.minute);
}
