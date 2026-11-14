typedef struct BoardSide {
    char data[0x24];
} BoardSide;

typedef struct Board {
    unsigned char active;
    char pad_001[0x107];
    BoardSide sides[2];
} Board;

extern int func_08AAD478(Board *self, BoardSide *side, int index);

int func_08AAC66C(Board *self, unsigned int side, int index)
{
    if (self->active == 0) {
        return -1;
    }
    if (side >= 2 || index < 0) {
        return -1;
    }
    return func_08AAD478(self, (BoardSide *)((char *)self->sides + side * sizeof(BoardSide)), index);
}
