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

int func_088D8418(Timer *timer)
{
    if (func_088D82B0(timer) == 0) {
        return -1;
    }
    timer->field_18 = 0;
    timer->field_1C = 0;
    timer->started = 0;
    timer->mode = 0;
    return 0;
}
