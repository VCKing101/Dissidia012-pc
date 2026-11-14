/* Loads the score board's model on first use and refreshes the board. */
typedef struct ScoreModel {
    unsigned char open;
    unsigned char pad_01[3];
    void *model;
    unsigned char pad_08[0xE8];
    void *source;
    int name;
} ScoreModel;

extern void *func_088DAA40(void *source, int name, int flags);
extern void func_089FC3E4(ScoreModel *board);

int func_089FC2A0(ScoreModel *board)
{
    if (!board->open) {
        return -1;
    }
    if (board->model == 0) {
        board->model = func_088DAA40(board->source, board->name, 0);
        if (board->model == 0) {
            return -2;
        }
    }
    func_089FC3E4(board);
    return 0;
}
