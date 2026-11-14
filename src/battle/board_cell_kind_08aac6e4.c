typedef struct BoardCell {
    char pad_00[0x1];
    signed char kind;
} BoardCell;

typedef struct Board {
    unsigned char active;
} Board;

extern BoardCell *func_08AAC3EC(Board *self, unsigned int x, unsigned int y);
extern int func_08AAC758(Board *self, int kind);

int func_08AAC6E4(Board *self, unsigned int x, unsigned int y)
{
    BoardCell *cell;

    if (self->active == 0) {
        return 0;
    }
    if ((int)(y | x) < 0) {
        return 0;
    }
    cell = func_08AAC3EC(self, x, y);
    if (cell == 0) {
        return 0;
    }
    return func_08AAC758(self, cell->kind);
}
