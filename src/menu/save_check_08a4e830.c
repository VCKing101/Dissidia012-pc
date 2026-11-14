typedef struct SaveFlags {
    char pad_00[0x13];
    unsigned char field_13;
} SaveFlags;

typedef struct SaveState {
    int field_00;
    int field_04;
    char pad_08[0x8];
    void *field_10;
    char field_14[0x6B0C];
} SaveState;

typedef struct SaveOwner {
    char pad_00[0x40];
    int field_40;
    char pad_44[0x18];
    SaveState *field_5C;
} SaveOwner;

extern SaveFlags D_09B6DCE8;
extern int func_08935490(const void *a, const void *b, int size);
extern void func_08A4E960(SaveOwner *self, int arg);
extern void func_08A4D07C(SaveOwner *self, int mode, float delay);

void func_08A4E830(SaveOwner *self, int arg)
{
    if (func_08935490(self->field_5C->field_14, self->field_5C->field_10, 0x6B0C) != 0) {
        D_09B6DCE8.field_13 = 1;
    }
    func_08A4E960(self, arg);
    self->field_40 = 2;
    func_08A4D07C(self, 1, 0.0f);
    self->field_5C->field_04 = 3;
}
