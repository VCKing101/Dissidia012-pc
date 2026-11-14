/* Opens the match board with its side and the eight resources it uses. */
typedef struct MatchBoard {
    unsigned char open;
    unsigned char pad_01[0x67];
    unsigned int side;
    void *source;
    void *layout;
    int round;
    void *frame;
    void *icons;
    void *digits;
    void *labels;
    void *marks;
} MatchBoard;

extern void func_089F8788(MatchBoard *board);

int func_089F8870(MatchBoard *board, unsigned int side, void *source, void *layout, int round, void *frame,
                  void *icons, void *digits, void *labels, void *marks)
{
    func_089F8788(board);
    if (side >= 2 || source == 0 || layout == 0 || round < 0 || frame == 0 || icons == 0 || digits == 0 ||
        labels == 0 || marks == 0) {
        return -1;
    }
    board->source = source;
    board->side = side;
    board->layout = layout;
    board->round = round;
    board->frame = frame;
    board->icons = icons;
    board->digits = digits;
    board->labels = labels;
    board->marks = marks;
    board->open = 1;
    return 0;
}
