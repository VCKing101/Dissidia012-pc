struct BoardPanel {
    char pad_00[0xC];
    short field_0C;
};

struct BoardState {
    char pad_0000[0x400];
    BoardPanel *field_400;
};

struct BoardOwner {
    char pad_0000[0x23F0];
    BoardState *field_23F0;
};

extern "C" {
bool func_08AECC14(BoardOwner *self)
{
    if (self->field_23F0->field_400->field_0C == 1) {
        return true;
    }
    return false;
}
}
