/* Resets the seven day slots of the calendar: today from the clock, the rest empty. */
typedef struct Calendar {
    char pad_00[0xC];
    unsigned char days[7];
} Calendar;

typedef struct SaveData {
    char pad_00000[0x1DE84];
    Calendar calendar;
} SaveData;

extern SaveData *D_089D712C;
extern int func_088F0CE4(void);
extern void func_088F0150(unsigned char *day, int value);

void func_088EFF24(void)
{
    Calendar *cal = &D_089D712C->calendar;
    int i;

    func_088F0150(&cal->days[0], func_088F0CE4());
    for (i = 1; i < 7; i++) {
        func_088F0150(&cal->days[i], 0);
    }
}
