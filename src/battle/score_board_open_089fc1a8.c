/* Opens the score board with its side and the resources it uses. */
typedef struct ScoreBoard {
    unsigned char open;
    unsigned char pad_01[0xE7];
    unsigned int side;
    void *source;
    void *layout;
    int round;
    int limit;
    void *frame;
    void *digits;
    void *marks;
} ScoreBoard;

extern void func_089FC0D8(ScoreBoard *board);

int func_089FC1A8(ScoreBoard *board, unsigned int side, void *source, void *layout, int round, int limit,
                  void *frame, void *digits, void *marks)
{
    func_089FC0D8(board);
    if (side >= 2 || source == 0 || layout == 0 || round < 0 || limit < 0 || frame == 0 || digits == 0 ||
        marks == 0) {
        return -1;
    }
    board->side = side;
    board->source = source;
    board->layout = layout;
    board->round = round;
    board->limit = limit;
    board->frame = frame;
    board->digits = digits;
    board->marks = marks;
    board->open = 1;
    return 0;
}
