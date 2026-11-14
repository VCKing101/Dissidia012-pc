typedef struct StateRequest {
    unsigned char active;
    char pad_001[0x3];
    int state;
    char pad_008[0x4];
    unsigned char request;
} StateRequest;

int func_08A17994(StateRequest *self)
{
    if (!self->active) {
        return -1;
    }
    if (self->state == 0 || self->state == 3) {
        self->request = 1;
    }
    return 0;
}
