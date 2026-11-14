typedef struct Flags0881A0E4 {
    union {
        unsigned int word;
        struct {
            unsigned int active : 1;
        } bits;
    } flags;
    float field_04;
} Flags0881A0E4;

int func_0881A0E4(Flags0881A0E4 *self) {
    if (!self->flags.bits.active) {
        return -1;
    }
    self->flags.word &= ~0x60000;
    self->field_04 = 0.0f;
    return 0;
}
