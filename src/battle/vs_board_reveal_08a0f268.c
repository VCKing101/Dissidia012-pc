/* After the reveal delay, loads the side's versus models and moves to
 * state 1. */
typedef struct VsBoardReveal {
    unsigned char open;
    unsigned char pad_01[3];
    int state;
    float time;
    void *frame;
    void *glow;
    unsigned char pad_14[0xF8];
    int owner;
    unsigned char pad_110[4];
    void *source;
} VsBoardReveal;

extern float D_08A0F8E8;
extern int D_08A0F8FC[2];
extern void func_088DA060(void *model);
extern unsigned int func_08A22CA8(int owner);
extern void *func_088DAA40(void *source, int name, int flags);

int func_08A0F268(VsBoardReveal *board, float step)
{
    board->time += step;
    if (board->time >= D_08A0F8E8) {
        unsigned int side;

        if (board->glow != 0) {
            func_088DA060(board->glow);
            board->glow = 0;
        }
        if (board->frame != 0) {
            func_088DA060(board->frame);
            board->frame = 0;
        }
        side = func_08A22CA8(board->owner);
        if (side >= 2) {
            return -1;
        }
        board->frame = func_088DAA40(board->source, D_08A0F8FC[side], 0);
        if (board->frame == 0) {
            return -2;
        }
        board->glow = func_088DAA40(board->source, 2, 0);
        if (board->glow == 0) {
            return -3;
        }
        board->state = 1;
    }
    return 0;
}
