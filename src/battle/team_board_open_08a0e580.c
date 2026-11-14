/* Opens the team board with its side and the resources it uses. */
typedef struct TeamBoard {
    unsigned char open;
    unsigned char pad_01[7];
    unsigned char parts[0x70];
    unsigned int side;
    void *source;
    void *layout;
    int round;
    int lead;
    void *frame;
    void *digits;
} TeamBoard;

extern void func_08A0E4A8(TeamBoard *board);
extern void func_089F7D74(void *parts, void *models, void *names);

int func_08A0E580(TeamBoard *board, unsigned int side, void *source, void *layout, void *models, void *names,
                  int round, int lead, void *frame, void *digits)
{
    func_08A0E4A8(board);
    if (side >= 2 || source == 0 || layout == 0 || models == 0 || names == 0 || round < 0 || lead < 0 ||
        frame == 0 || digits == 0) {
        return -1;
    }
    func_089F7D74(board->parts, models, names);
    board->source = source;
    board->side = side;
    board->layout = layout;
    board->round = round;
    board->lead = lead;
    board->frame = frame;
    board->digits = digits;
    board->open = 1;
    return 0;
}
