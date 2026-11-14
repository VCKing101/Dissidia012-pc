typedef struct ReadyFlag23960 {
    char pad_000[0xC];
    unsigned char field_0C;
} ReadyFlag23960;

extern unsigned char func_08A23454(ReadyFlag23960 *self);

int func_08A23960(ReadyFlag23960 *self)
{
    if (!func_08A23454(self)) {
        return -1;
    }
    self->field_0C = 0;
    return 0;
}

int func_08A2399C(ReadyFlag23960 *self)
{
    if (!func_08A23454(self)) {
        return -1;
    }
    self->field_0C = 1;
    return 0;
}
