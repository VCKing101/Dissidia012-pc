typedef struct SaveBlocks {
    void *field_00;
    void *field_04;
} SaveBlocks;

typedef struct SaveState {
    char pad_0000[0x7190];
    SaveBlocks *field_7190;
} SaveState;

typedef struct SaveOwner {
    char pad_00[0x5C];
    SaveState *field_5C;
} SaveOwner;

extern void func_08935524(void *dst, const void *src, int size);

int func_08A4DA2C(SaveOwner *self, SaveBlocks *out)
{
    if (self->field_5C->field_7190 == 0) {
        return 0;
    }
    func_08935524(out->field_00, self->field_5C->field_7190->field_00, 0x160);
    func_08935524(out->field_04, self->field_5C->field_7190->field_04, 0x3FC);
    return 1;
}
