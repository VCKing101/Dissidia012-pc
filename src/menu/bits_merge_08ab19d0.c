/* Merges another unlock set into this one (bitwise or). */
typedef struct MergeBits {
    char pad_000[0x44];
    unsigned int field_44;
    unsigned int field_48[32];
    unsigned int field_C8[32];
    unsigned int field_148[2];
} MergeBits;

void func_08AB19D0(MergeBits *self, MergeBits *other)
{
    int i;
    self->field_44 = self->field_44 | other->field_44;
    for (i = 0; i < 32; i++) {
        self->field_48[i] = self->field_48[i] | other->field_48[i];
    }
    for (i = 0; i < 32; i++) {
        self->field_C8[i] = self->field_C8[i] | other->field_C8[i];
    }
    for (i = 0; i < 2; i++) {
        self->field_148[i] = self->field_148[i] | other->field_148[i];
    }
}
