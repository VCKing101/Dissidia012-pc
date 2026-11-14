typedef struct TimerB {
    int field_00;
    int mode;
    int value;
    int limit;
    int field_10;
    unsigned char started;
} TimerB;

extern int func_088DF2B8(TimerB *timer);

int func_088DF3F8(TimerB *timer)
{
    if (func_088DF2B8(timer) == 0) {
        return -1;
    }
    timer->field_10 = 0;
    timer->started = 0;
    timer->mode = 0;
    return 0;
}
