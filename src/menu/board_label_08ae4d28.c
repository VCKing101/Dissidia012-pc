typedef struct BoardState {
    char pad_0000[0x400];
    char field_400[0x2568 - 0x400];
    int field_2568;
} BoardState;

typedef struct BoardOwner {
    char pad_00000[0xA690];
    BoardState *field_A690;
} BoardOwner;

extern unsigned short D_08AEBC48[];
extern void func_088D3AE8(char *text, int x, int id, int y);

void func_08AE4D28(BoardOwner *self)
{
    BoardState *state = self->field_A690;
    func_088D3AE8(state->field_400, 0x22, D_08AEBC48[state->field_2568], 0x1F);
}
