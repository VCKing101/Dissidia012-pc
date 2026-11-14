typedef struct MenuRootState {
    char pad_0000[0x6968];
    int field_6968;
    char pad_696C[0x8E60 - 0x696C];
    unsigned int field_8E60;
} MenuRootState;

typedef struct MenuRootOwner {
    char pad_000[0x154];
    MenuRootState *field_154;
} MenuRootOwner;

typedef struct MenuRootFlags {
    char pad_00[0x18];
    unsigned char field_18;
} MenuRootFlags;

extern MenuRootFlags D_09B6D840;

void func_08A1C88C(MenuRootOwner *self) {
    self->field_154->field_8E60 &= ~0x18;
    self->field_154->field_8E60 |= 0x20;
    D_09B6D840.field_18 = 1;
    self->field_154->field_6968 = 9;
}

void func_08A1C8DC(MenuRootOwner *self) {
    self->field_154->field_8E60 &= ~0x18;
    D_09B6D840.field_18 = 0;
    self->field_154->field_6968 = 0;
}
