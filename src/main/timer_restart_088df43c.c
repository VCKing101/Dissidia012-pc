typedef struct TimerB {
    int field_00;
    int mode;
    int value;
} TimerB;

extern int func_088DF2B8(TimerB *timer);

int func_088DF43C(TimerB *timer)
{
    if (func_088DF2B8(timer) == 0) {
        return -1;
    }
    if (timer->mode == 1) {
        timer->value = 2;
        timer->mode = 2;
    }
    return 0;
}
