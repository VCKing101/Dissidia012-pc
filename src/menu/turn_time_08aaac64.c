/* Loads the turn time for the current rule and side; flag 0x800 marks an unlimited turn. */
typedef struct TimeBoard {
    char pad_0000[0x7CC0];
    unsigned int field_7CC0;
    char pad_7CC4[0x7CD0 - 0x7CC4];
    int field_7CD0;
    int field_7CD4;
} TimeBoard;

typedef struct TimeOwner {
    char pad_0000[0x7CFC];
    TimeBoard *field_7CFC;
} TimeOwner;

extern short D_08AB0934[];

void func_08AAAC64(TimeOwner *self)
{
    int side = 0;
    TimeBoard *board = self->field_7CFC;
    if ((board->field_7CC0 & 2) != 0) {
        side = 1;
    }
    board->field_7CD0 = D_08AB0934[board->field_7CD4 * 2 + side];
    self->field_7CFC->field_7CC0 = self->field_7CFC->field_7CC0 & ~0x800;
    board = self->field_7CFC;
    if (board->field_7CD0 == 0) {
        board->field_7CC0 = board->field_7CC0 | 0x800;
    }
}
