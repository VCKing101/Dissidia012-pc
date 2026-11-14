typedef struct Timer {
    char pad_00[0xC];
    int mode;
    int value;
    int limit;
    int field_18;
    int field_1C;
    unsigned char started;
} Timer;

extern int func_088D82B0(Timer *timer);

int func_088D83D8(Timer *timer)
{
    if (func_088D82B0(timer) == 0) {
        return -1;
    }
    timer->started = 1;
    return 0;
}
