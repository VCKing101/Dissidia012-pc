typedef struct RangeState {
    char pad_00[0x4];
    short kind;
    char pad_06[0x4E];
    float distance;
} RangeState;

/* 1 for kinds 1 and 2 while the distance is under 14. */
unsigned char func_08ABC6D0(RangeState *self)
{
    int result = 0;
    if ((self->kind == 1 || self->kind == 2) && self->distance < 14.0f) {
        result = 1;
    }
    return result;
}
