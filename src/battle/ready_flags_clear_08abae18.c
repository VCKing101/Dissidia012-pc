typedef struct ReadyBlock {
    char pad_000[0xD8];
    unsigned char flags[4];
    char pad_0DC[0x104];
    float values[4];
} ReadyBlock;

void func_08ABAE18(ReadyBlock *self)
{
    self->flags[0] = 0;
    self->flags[1] = 0;
    self->flags[2] = 0;
    self->flags[3] = 0;
    self->values[0] = 0.0f;
    self->values[1] = 0.0f;
    self->values[2] = 0.0f;
    self->values[3] = 0.0f;
}
