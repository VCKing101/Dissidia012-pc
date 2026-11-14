/* Timer accessors guarded by func_088DF2B8. */
typedef struct TimerB {
    int field_00;
    int mode;
    int value;
    int limit;
    int field_10;
    unsigned char started;
} TimerB;

extern int func_088DF2B8(TimerB *timer);

int func_088DF2C0(TimerB *timer)
{
    if (func_088DF2B8(timer) == 0) {
        return 0;
    }
    return timer->value;
}
