typedef struct ApproachTask {
    void *vtable;
    unsigned char bound;
    char pad_05[0x3];
    int handle;
    float best;
    int stall;
} ApproachTask;

extern float func_08A896BC(ApproachTask *self);

/* True once the distance has not improved for 120 updates. */
int func_08A89658(ApproachTask *self)
{
    float distance = func_08A896BC(self);
    int result = 0;

    if (distance < self->best) {
        self->stall = 0;
        self->best = distance;
    } else {
        int stall = self->stall + 1;
        self->stall = stall;
        if (stall >= 120) {
            result = 1;
        }
    }
    return result;
}
