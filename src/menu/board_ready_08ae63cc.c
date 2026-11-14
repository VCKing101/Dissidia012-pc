typedef struct BoardPanel {
    char pad_00[0xC];
    short field_0C;
} BoardPanel;

typedef struct BoardState {
    char pad_0000[0x400];
    BoardPanel *field_400;
} BoardState;

typedef struct BoardOwner {
    char pad_00000[0xA690];
    BoardState *field_A690;
} BoardOwner;

int func_08AE63CC(BoardOwner *self)
{
    if (self->field_A690->field_400->field_0C == 2) {
        return 1;
    }
    return 0;
}
