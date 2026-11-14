typedef struct BoardCell {
    signed char field_00;
    signed char field_01;
    signed char field_02;
    signed char field_03;
    signed char field_04;
    signed char field_05;
} BoardCell;

typedef struct BoardRow {
    BoardCell field_00;
    BoardCell field_06[8];
    char pad_36[4];
} BoardRow;

typedef struct BoardGlobal {
    char pad_000[0x148];
    BoardRow field_148[8];
} BoardGlobal;

typedef struct BoardCommand {
    BoardCell field_00;
    short field_06;
} BoardCommand;

extern BoardGlobal D_09B6DF08;
extern void func_08A71D98(void *self, int arg);

void func_08A718C4(void *self, int arg, BoardCommand *command)
{
    D_09B6DF08.field_148[0].field_00 = command->field_00;
    func_08A71D98(self, arg);
}
