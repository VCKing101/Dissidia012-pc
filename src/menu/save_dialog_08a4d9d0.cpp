typedef struct MenuSaveState {
    char pad_0000[0x6FD8];
    int field_6FD8;
    char pad_6FDC[0x6FF8 - 0x6FDC];
    int field_6FF8;
    char pad_6FFC[0x7190 - 0x6FFC];
    int field_7190;
    int field_7194;
    unsigned char field_7198;
} MenuSaveState;

typedef struct MenuSaveOwner {
    char pad_00[0x5C];
    MenuSaveState *field_5C;
} MenuSaveOwner;

extern "C" void func_08A697B0(int handle, int mode, int value);
extern "C" void func_08A622FC(int handle, int mode);

extern "C" bool func_08A4D9D0(MenuSaveOwner *self, int value) {
    func_08A697B0(self->field_5C->field_6FF8, 3, -1);
    self->field_5C->field_7190 = value;
    func_08A622FC(self->field_5C->field_6FD8, 1);
    return true;
}
