/* Opens the stage board with its sides and the resources it uses. */
typedef struct StageBoard {
    unsigned char open;
    unsigned char pad_01[0x17];
    unsigned int side;
    unsigned int other;
    void *source;
    void *layout;
    int round;
    void *frame;
    int limit;
    void *digits;
} StageBoard;

extern void func_08A00784(StageBoard *board);

int func_08A00804(StageBoard *board, unsigned int side, unsigned int other, void *source, void *layout, int round,
                  void *frame, int limit, void *digits)
{
    func_08A00784(board);
    if (side >= 2 || other >= 2 || source == 0 || layout == 0 || round < 0 || frame == 0 || limit < 0 ||
        digits == 0) {
        return -1;
    }
    board->side = side;
    board->other = other;
    board->source = source;
    board->layout = layout;
    board->round = round;
    board->frame = frame;
    board->limit = limit;
    board->digits = digits;
    board->open = 1;
    return 0;
}
