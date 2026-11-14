typedef struct BoardState {
    char pad_0000[0x25C0];
    int field_25C0;
    char pad_25C4[0x2698 - 0x25C4];
    int field_2698;
    int pad_269C;
    int field_26A0[8];
    char pad_26C0[0x7EC0 - 0x26C0];
    unsigned short field_7EC0[8][0x2BF];
} BoardState;

typedef struct BoardOwner {
    char pad_00000[0xA690];
    BoardState *field_A690;
} BoardOwner;

extern int func_08AE5984(BoardOwner *self, int row, int id, int flags);

int func_08AE5A44(BoardOwner *self, int column)
{
    BoardState *state = self->field_A690;
    int row = state->field_2698;
    if (state->field_25C0 < state->field_26A0[row]) {
        return func_08AE5984(self, row, state->field_7EC0[row][column], 0);
    }
    return -4;
}
