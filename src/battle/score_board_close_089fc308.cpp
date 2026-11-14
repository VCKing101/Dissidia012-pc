/* Releases every model of the score board. */
struct ScoreRow {
    void *cells[13];
};

struct ScoreBoardModels {
    unsigned char open;
    unsigned char pad_01[3];
    void *model;
    void *labels[4];
    ScoreRow rows[4];
};

extern "C" {

extern void func_088DA060(void *model);

int func_089FC308(ScoreBoardModels *board)
{
    if (!board->open) {
        return -1;
    }
    for (int row = 0; row < 4; row++) {
        for (int cell = 0; cell < 13; cell++) {
            if (board->rows[row].cells[cell] != 0) {
                func_088DA060(board->rows[row].cells[cell]);
                board->rows[row].cells[cell] = 0;
            }
        }
        if (board->labels[row] != 0) {
            func_088DA060(board->labels[row]);
            board->labels[row] = 0;
        }
    }
    if (board->model != 0) {
        func_088DA060(board->model);
        board->model = 0;
    }
    return 0;
}

}
