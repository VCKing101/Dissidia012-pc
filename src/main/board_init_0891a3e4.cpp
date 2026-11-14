extern "C" {
extern void func_0891A724(void *group);
extern void func_0891A77C(void *grid);
extern void func_0891A7E4(void *ids);

typedef struct CardGroup {
    char data[0x68];
} CardGroup;

typedef struct CardBoard {
    int field_000;
    int field_004;
    CardGroup groups[2];        /* 0x008 */
    char grid[0x1F8 - 0xD8];    /* 0x0D8 */
    char ids[0x2C];             /* 0x1F8 */
} CardBoard;

void func_0891A3E4(CardBoard *board)
{
    board->field_000 = 0;
    board->field_004 = 0;
    for (int i = 0; i < 2; i++) {
        func_0891A724(&board->groups[i]);
    }
    func_0891A77C(board->grid);
    func_0891A7E4(board->ids);
}
}
