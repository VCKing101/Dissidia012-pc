typedef struct SaveFlags {
    char pad_00[0x13];
    unsigned char field_13;
    unsigned char field_14;
    unsigned char field_15;
} SaveFlags;

typedef struct SaveState {
    char pad_0000[0x6B28];
    void *field_6B28;
    void *field_6B2C;
} SaveState;

typedef struct SaveOwner {
    char pad_00[0x5C];
    SaveState *field_5C;
} SaveOwner;

extern SaveFlags D_09B6DCE8;
extern void func_08935524(void *dst, const void *src, int size);

void func_08A4E7CC(SaveOwner *self, const void *header, const void *title)
{
    SaveFlags *flags = &D_09B6DCE8;
    func_08935524(self->field_5C->field_6B28, header, 0x10);
    func_08935524(self->field_5C->field_6B2C, title, 0x66);
    flags->field_15 = 0;
}
