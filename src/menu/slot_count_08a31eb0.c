/* Counts the used slots (id != 0xFFFF) and remembers the first used one. */
typedef struct CountState {
    char pad_000[0x164];
    int field_164;
    int field_168;
    char pad_16C[0x17C - 0x16C];
    unsigned short field_17C[13];
} CountState;

typedef struct CountOwner {
    char pad_00[0x48];
    CountState *field_48;
} CountOwner;

void func_08A31EB0(CountOwner *self)
{
    int i;
    self->field_48->field_168 = 0;
    self->field_48->field_164 = 0;
    for (i = 0; i < 13; i++) {
        if (self->field_48->field_17C[i] != 0xFFFF) {
            self->field_48->field_168 = self->field_48->field_168 + 1;
        }
    }
    for (i = 0; i < 13; i++) {
        if (self->field_48->field_17C[i] != 0xFFFF) {
            self->field_48->field_164 = i;
            break;
        }
    }
}
