typedef struct BoardState {
    char pad_0000[0x400];
    char field_400[0x25C0 - 0x400];
    int field_25C0;
    char pad_25C4[8];
    int field_25CC;
} BoardState;

typedef struct BoardOwner {
    char pad_00000[0xA690];
    BoardState *field_A690;
} BoardOwner;

extern unsigned short D_08AEBC58[];
extern void func_088D3AE8(char *text, int x, int id, int y);

void func_08AE4EBC(BoardOwner *self)
{
    BoardState *state = self->field_A690;
    func_088D3AE8(state->field_400, 0x41, D_08AEBC58[state->field_25C0 - state->field_25CC], 0x21);
}
