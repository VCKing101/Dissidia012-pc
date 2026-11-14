/* Whether member `index` can still move and its best move reaches the current target.
   Retail stores the move result in the frame; a two-word local array reproduces that. */
typedef struct ReadyMove {
    int field_00;
    int field_04;
} ReadyMove;

typedef struct ReadyBoard {
    char pad_0000[0x788];
    ReadyMove field_788[5];
    char pad_07B0[0x7CCC - 0x7B0];
    void *field_7CCC;
} ReadyBoard;

typedef struct ReadyOwner3 {
    char pad_0000[0x7CFC];
    ReadyBoard *field_7CFC;
} ReadyOwner3;

extern int func_08AB47BC(void *board);
extern int func_08AB48F4(void *board);
extern int func_08AB45CC(void *board, int zero, int from, int index);

int func_08AAB9D8(ReadyOwner3 *self, int index)
{
    int moves = self->field_7CFC->field_788[index].field_00;
    int from = func_08AB47BC(self->field_7CFC->field_7CCC);
    int target = func_08AB48F4(self->field_7CFC->field_7CCC);
    if (moves > 0) {
        int reached[2];
        reached[0] = func_08AB45CC(self->field_7CFC->field_7CCC, 0, from, index);
        if (target == reached[0]) {
            return 1;
        }
    }
    return 0;
}
