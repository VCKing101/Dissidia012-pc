/* Timer accessors guarded by func_088D82B0. */
typedef struct Timer {
    char pad_00[0xC];
    int mode;
    int value;
    int limit;
} Timer;

extern int func_088D82B0(Timer *timer);

int func_088D82B8(Timer *timer)
{
    if (func_088D82B0(timer) == 0) {
        return 0;
    }
    return timer->value;
}
