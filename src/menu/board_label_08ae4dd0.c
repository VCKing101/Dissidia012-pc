typedef struct BoardState {
    char pad_0000[0x400];
    char field_400[0x2594 - 0x400];
    int field_2594;
} BoardState;

typedef struct BoardOwner {
    char pad_00000[0xA690];
    BoardState *field_A690;
} BoardOwner;

extern unsigned short D_08AEBC50[];
extern void func_088D3AE8(char *text, int x, int id, int y);

void func_08AE4DD0(BoardOwner *self)
{
    BoardState *state = self->field_A690;
    int text = D_08AEBC50[state->field_2594];
    func_088D3AE8(state->field_400, 0x24, text, 0x20);
}
