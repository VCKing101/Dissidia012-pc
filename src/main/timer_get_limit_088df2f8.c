typedef struct TimerB {
    int field_00;
    int mode;
    int value;
    int limit;
} TimerB;

extern int func_088DF2B8(TimerB *timer);

int func_088DF2F8(TimerB *timer)
{
    if (func_088DF2B8(timer) == 0) {
        return -1;
    }
    return timer->limit;
}
