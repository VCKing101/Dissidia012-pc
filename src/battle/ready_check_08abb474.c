typedef struct ReadyFlags {
    unsigned char ready_00;
    char pad_01[0x5F];
    unsigned char ready_60;
    char pad_61[0x53];
    unsigned char ready_B4;
    char pad_B5[0x23];
    unsigned char ready_D8;
} ReadyFlags;

/* 1 when all four ready bytes are 1. */
unsigned char func_08ABB474(ReadyFlags *self)
{
    int result = 0;
    if (self->ready_D8 == 1 && self->ready_00 == 1 && self->ready_60 == 1 && self->ready_B4 == 1) {
        result = 1;
    }
    return result;
}
