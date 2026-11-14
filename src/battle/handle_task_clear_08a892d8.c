typedef struct HandleTaskState {
    void *vtable;
    unsigned char bound;
    char pad_05[0x3];
    int handle;
    float best;
    int stall;
} HandleTaskState;

void func_08A892D8(HandleTaskState *self)
{
    self->bound = 0;
    self->handle = 0;
    self->best = 0.0f;
    self->stall = 0;
}
