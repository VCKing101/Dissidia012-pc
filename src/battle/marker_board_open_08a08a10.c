/* Opens a marker board with the resources it uses. */
typedef struct MarkerBoard {
    unsigned char open;
    unsigned char pad_01[0xF];
    void *source;
    void *layout;
    int round;
    int lead;
    int limit;
    void *frame;
    void *digits;
} MarkerBoard;

extern void func_08A08990(MarkerBoard *board);

int func_08A08A10(MarkerBoard *board, void *source, void *layout, int round, int lead, int limit, void *frame,
                  void *digits)
{
    func_08A08990(board);
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
