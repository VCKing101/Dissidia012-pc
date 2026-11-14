/*
 * Word 0 packs: bit 0 enabled, bits 1-16 value, bits 17-18 mode.
 * Reset clears enabled, sets value to 0xFFFF and mode to 3.
 */
typedef struct Agent4Bits {
    unsigned int bits;
    float field_04;
} Agent4Bits;

void func_08819ED0(Agent4Bits *self) {
    unsigned int bits = self->bits & ~1;
    bits |= 0xFFFF << 1;
    bits |= 3 << 17;
    self->bits = bits;
    self->field_04 = 0.0f;
}
