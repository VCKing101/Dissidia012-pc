typedef struct BoardState {
    char pad_0000[0x2694];
    int field_2694;
    char pad_2698[0x7E98 - 0x2698];
    short field_7E98[5][4];
} BoardState;

typedef struct BoardOwner {
    char pad_00000[0xA690];
    BoardState *field_A690;
} BoardOwner;

extern void func_08AE7B44(BoardOwner *self);
extern void func_08AE68A8(BoardOwner *self);
extern void func_08AE6CFC(BoardOwner *self, int value);

void func_08AE76E8(BoardOwner *self)
{
    int i;
    for (i = 0; i < 4; i++) {
        self->field_A690->field_7E98[self->field_A690->field_2694][i] = 0x7FFF;
    }
    func_08AE7B44(self);
    func_08AE68A8(self);
    func_08AE6CFC(self, 0);
}
