typedef struct Timer {
    char pad_00[0xC];
    int mode;
    int value;
    int limit;
} Timer;

extern int func_088D82B0(Timer *timer);

int func_088D82F0(Timer *timer)
{
    if (func_088D82B0(timer) == 0) {
        return -1;
    }
    return timer->limit;
}
