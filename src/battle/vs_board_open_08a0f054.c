/* Starts the versus board: shows the score gauge of each side still
 * playing and records each side's state and score. */
typedef struct VsGauge {
    unsigned char data[0x38];
} VsGauge;

typedef struct VsBoard {
    unsigned char open;
    unsigned char pad_01[3];
    int state;
    float time;
    unsigned char pad_0c[0x28];
    VsGauge gauges[2];
    unsigned char pad_a4[0x68];
    int turn;
    unsigned char pad_110[0xC];
    unsigned char done[2];
    unsigned char pad_11e[2];
    int scores[2];
} VsBoard;

extern int func_089E99C0(int turn, int score);
extern void func_089F84E0(VsGauge *gauge);
extern void func_089F85B0(VsGauge *gauge, int score, int value);
extern void func_08A0EFD0(VsBoard *board);

int func_08A0F054(VsBoard *board, unsigned char *done, int *scores)
{
    if (!board->open) {
        return -1;
    }
    if (done == 0 || scores == 0) {
        return -2;
    }
    {
        int i;
        for (i = 0; i < 2; i++) {
            int turn;
            int score;
            int value;

            if (done[i] == 1) {
                continue;
            }
            turn = (board->turn + i) % 2;
            score = scores[i];
            value = func_089E99C0(turn, score);
            func_089F84E0(&board->gauges[i]);
            func_089F85B0(&board->gauges[i], score, value);
        }
    }
    {
        int i;
        for (i = 0; i < 2; i++) {
            board->done[i] = done[i];
            board->scores[i] = scores[i];
        }
    }
    board->state = 0;
    board->time = 0.0f;
    func_08A0EFD0(board);
    return 0;
}
