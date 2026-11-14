typedef struct MenuRootState {
    char pad_0000[0x8E60];
    unsigned int field_8E60;
} MenuRootState;

typedef struct MenuRootOwner {
    char pad_000[0x154];
    MenuRootState *field_154;
} MenuRootOwner;

int func_08A1D8E4(MenuRootOwner *self) {
    return (self->field_154->field_8E60 & 1) != 0 ? 1 : 0;
}
