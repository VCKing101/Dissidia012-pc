/* Opens the chase board with the resources it uses. */
typedef struct ChaseBoard {
    unsigned char open;
    unsigned char pad_01[0x13];
    void *source;
    void *layout;
    int round;
    int lead;
    int limit;
    void *frame;
    void *digits;
} ChaseBoard;

extern void func_089FDEF8(ChaseBoard *board);

int func_089FDF78(ChaseBoard *board, void *source, void *layout, int round, int lead, int limit, void *frame,
                  void *digits)
{
    func_089FDEF8(board);
    if (source == 0 || layout == 0 || round < 0 || lead < 0 || limit < 0 || frame == 0 || digits == 0) {
        return -1;
    }
    board->source = source;
    board->layout = layout;
    board->round = round;
    board->lead = lead;
    board->limit = limit;
    board->frame = frame;
    board->digits = digits;
    board->open = 1;
    return 0;
}
