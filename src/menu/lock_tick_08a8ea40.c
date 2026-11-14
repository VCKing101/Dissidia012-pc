/* Counts the lock timer down and keeps flag 0x20000 set while it runs. */
typedef struct LockTimer {
    char pad_000[0x2A8];
    unsigned int field_2A8;
    char pad_2AC[0x2F8 - 0x2AC];
    int field_2F8;
} LockTimer;

typedef struct LockTimerOwner {
    char pad_00[0x8];
    LockTimer *field_08;
} LockTimerOwner;

void func_08A8EA40(LockTimerOwner *self)
{
    LockTimer *timer;
    self->field_08->field_2F8 = self->field_08->field_2F8 - 1;
    timer = self->field_08;
    if (timer->field_2F8 <= 0) {
        timer->field_2F8 = 0;
        timer = self->field_08;
    }
    timer->field_2A8 = timer->field_2A8 & ~0x20000;
    timer = self->field_08;
    if (timer->field_2F8 > 0) {
        timer->field_2A8 = timer->field_2A8 | 0x20000;
    }
}
