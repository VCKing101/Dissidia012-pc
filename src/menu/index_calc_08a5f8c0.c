typedef struct A6Grid {
    char pad_000[0x328];
    int field_328;
    char pad_32C[0x28];
    int field_354;
} A6Grid;

int func_08A5F8C0(A6Grid *self) {
    int v = self->field_328 * 13 + self->field_354;
    if (v < 0 || v > 0x5A) {
        v = 0;
    }
    return v;
}
