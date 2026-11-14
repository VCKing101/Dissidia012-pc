typedef struct MenuCardState {
    char pad_0000[0xBB0];
    int field_BB0;
    char pad_0BB4[0x8];
    int field_BBC;
    char pad_0BC0[0x1048 - 0xBC0];
    unsigned int field_1048[1];
} MenuCardState;

typedef struct MenuCardOwner {
    char pad_000[0x138];
    MenuCardState *field_138;
} MenuCardOwner;

extern "C" {
void func_088D4BF4(int sound);

bool func_08A2B190(MenuCardOwner *self) {
    MenuCardState *state = self->field_138;
    switch (state->field_1048[state->field_BB0 - state->field_BBC] & 0x1F) {
    case 0:
    case 1:
    case 2:
    case 3:
        func_088D4BF4(4);
        return false;
    case 4:
        func_088D4BF4(1);
        return true;
    default:
        return false;
    }
}
}
