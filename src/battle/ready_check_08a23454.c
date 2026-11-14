typedef struct ReadyCheck23454 {
    unsigned char field_00;
    char pad_001[0xF];
    unsigned char field_10;
    char pad_011[0x6F];
    unsigned char field_80;
} ReadyCheck23454;

unsigned char func_08A23454(ReadyCheck23454 *self)
{
    int result = 0;
    if (self->field_00 == 1 && self->field_10 == 1 && self->field_80 == 1) {
        result = 1;
    }
    return result;
}
