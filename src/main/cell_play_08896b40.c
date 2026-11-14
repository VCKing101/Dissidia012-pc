/* Plays the sound of cell [row][col][slot] unless the bank is unloaded (-1). */
typedef struct Row08896B40 {
    char pad_00[0x20];
    int sounds[7][4];
} Row08896B40;

typedef struct Bank08896B40 {
    int field_00;
    Row08896B40 *rows;
    int field_08;
    int id;
} Bank08896B40;

int func_088A24D4(int one, int sound, int one2, int zero);

int func_08896B40(Bank08896B40 *b, int row, int col, int slot)
{
    if (b->id == -1) {
        return 0;
    }
    return func_088A24D4(1, b->rows[row].sounds[col][slot], 1, 0);
}
