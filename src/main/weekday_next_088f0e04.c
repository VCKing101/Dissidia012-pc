/* Day of the week after today (0-6). */
extern int func_088F0C80(void);

int func_088F0E04(void)
{
    int day = func_088F0C80() + 1;

    if (day >= 7) {
        day = 0;
    }
    return day;
}
