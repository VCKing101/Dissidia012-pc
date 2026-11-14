typedef struct PhaseState {
    unsigned char active;
    char pad_01[0xF];
    int phase;
    char pad_14[0x18];
    int request;
} PhaseState;

extern void *func_08899300(void);
extern int func_08897998(void *queue, int request);

/* 1 when idle and the request is still queued. */
int func_08AC1388(PhaseState *self)
{
    if (self->active == 0) {
        return 0;
    }
    if (self->phase != 0) {
        return 0;
    }
    if (func_08897998(func_08899300(), self->request) == 0) {
        return 0;
    }
    return 1;
}
