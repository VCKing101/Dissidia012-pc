typedef struct MenuSaveState {
    char pad_0000[0x6FD8];
    int field_6FD8;
    char pad_6FDC[0x6FF8 - 0x6FDC];
    int field_6FF8;
    char pad_6FFC[0x7194 - 0x6FFC];
    int field_7194;
    unsigned char field_7198;
} MenuSaveState;

typedef struct MenuSaveOwner {
    char pad_00[0x5C];
    MenuSaveState *field_5C;
} MenuSaveOwner;

extern void func_08A697B0(int handle, int mode, int value);
extern void func_08A622FC(int handle, int mode);

int func_08A4DA98(MenuSaveOwner *self, int value, unsigned char overwrite) {
    func_08A697B0(self->field_5C->field_6FF8, 3, -1);
    self->field_5C->field_7194 = value;
    self->field_5C->field_7198 = overwrite;
    if (overwrite != 0) {
        func_08A622FC(self->field_5C->field_6FD8, 3);
    } else {
        func_08A622FC(self->field_5C->field_6FD8, 2);
    }
    return 1;
}
