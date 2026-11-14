typedef struct CommandState {
    unsigned char active;
    char pad_01[0x21F];
    void *target;
} CommandState;

int func_08A87AA4(CommandState *self, void *target)
{
    if (self->active == 0) {
        return -1;
    }
    if (target == 0) {
        return -2;
    }
    self->target = target;
    return 0;
}
