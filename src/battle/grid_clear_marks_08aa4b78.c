typedef struct GridCell {
    unsigned char a;
    unsigned char b;
} GridCell;

typedef struct GridState {
    char pad_00[0xC];
    GridCell *cells;
    char pad_10[0x4];
    int field_14;
    int markCount;
    short *marks;
} GridState;

/* Clears every marked cell, then the mark list. */
void func_08AA4B78(GridState *self)
{
    int i;

    for (i = 0; i < self->markCount; i++) {
        int mark = self->marks[i];
        self->cells[mark].a = 0;
        self->cells[mark].b = 0;
    }
    self->field_14 = 0;
    self->markCount = 0;
}
