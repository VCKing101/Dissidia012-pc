struct BoardCell {
    signed char data[6];
};

struct BoardRow {
    BoardCell field_00;
    BoardCell field_06[8];
    char pad_36[4];

    static BoardCell *cells(BoardRow *row) { return row->field_06; }
};

struct BoardGlobal {
    char pad_000[0x148];
    BoardRow field_148[8];
};

extern "C" BoardGlobal D_09B6DF08;
extern "C" void func_08A707DC(BoardCell *cells);
extern "C" void func_08A71D98(void *self, int arg);

extern "C" {
void func_08A71BC0(void *self, int arg, short *row)
{
    BoardRow *rows = D_09B6DF08.field_148;
    func_08A707DC(BoardRow::cells(&rows[*row]));
    func_08A71D98(self, arg);
}
}
