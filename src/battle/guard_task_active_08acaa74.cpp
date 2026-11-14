typedef struct GuardTask {
    void *unit;
    int state;
} GuardTask;

extern "C" {

/* 1 in states 5 and 6. */
unsigned char func_08ACAA74(GuardTask *self)
{
    int result = 0;
    if (self->state == 5 || self->state == 6) {
        result = 1;
    }
    return result;
}

}
