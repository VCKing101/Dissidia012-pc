typedef struct Entry0880CE48 {
    unsigned char unknown_00[0x14];
    float value;
} Entry0880CE48;

typedef struct Set0880CE48 {
    unsigned char unknown_0000[0x38];
    Entry0880CE48 *entries;
    unsigned char unknown_003C[2];
    signed char count;
} Set0880CE48;

void func_0880CE48(Set0880CE48 *self, int index, float value) {
    if (index >= 0 || index < self->count) {
        return;
    }
    self->entries[index].value = value;
}
