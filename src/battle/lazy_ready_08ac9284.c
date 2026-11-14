typedef struct LazyOwner {
    unsigned char active;
    char pad_01[0x1F];
    unsigned char ready;
} LazyOwner;

/* 1 when the owner is active and ready. */
unsigned char func_08AC9284(LazyOwner *self)
{
    int result = 0;
    if (self->active == 1 && self->ready == 1) {
        result = 1;
    }
    return result;
}
