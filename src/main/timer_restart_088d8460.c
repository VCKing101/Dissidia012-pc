typedef struct Timer {
    char pad_00[0xC];
    int mode;
    int value;
} Timer;

extern int func_088D82B0(Timer *timer);

int func_088D8460(Timer *timer)
{
    if (func_088D82B0(timer) == 0) {
        return -1;
    }
    if (timer->mode == 1) {
        timer->value = 2;
        timer->mode = 2;
    }
    return 0;
}
