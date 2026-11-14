/* Shows a side's score on the versus board as up to four digit models,
 * refreshes the side gauge and lights the side's pad. */
typedef struct VsGauge {
    unsigned char data[0x38];
} VsGauge;

typedef struct VsMarks {
    void *marks[4];
} VsMarks;

typedef struct VsBoardDigits {
    unsigned char open;
    unsigned char pad_01[0x13];
    VsMarks sides[2];
    VsGauge gauges[2];
    unsigned char pad_a4[0x68];
    int turn;
    int pad_index;
    unsigned char pad_114[4];
    void *source;
    unsigned char pad_11c[4];
    int scores[2];
} VsBoardDigits;

extern int D_08A0F94C[10];
extern void func_088DA060(void *model);
extern void *func_088DAA40(void *source, int name, int flags);
extern void func_089F854C(VsGauge *gauge);
extern void func_08889F28(int side, int pad);

int func_08A0F784(VsBoardDigits *board, unsigned int side)
{
    int score;

    if (side >= 2) {
        return -1;
    }
    score = board->scores[side];
    if (score < 0) {
        score = 0;
    } else if (score >= 10000) {
        score = 9999;
    }
    {
        int i;
        for (i = 0; i < 4; i++) {
            if (board->sides[side].marks[i] != 0) {
                func_088DA060(board->sides[side].marks[i]);
                board->sides[side].marks[i] = 0;
            }
            if (score == 0 && i != 0) {
                break;
            }
            {
                int digit = score % 10;
                score /= 10;
                board->sides[side].marks[i] = func_088DAA40(board->source, D_08A0F94C[digit], 0);
            }
        }
    }
    func_089F854C(&board->gauges[side]);
    func_08889F28((board->turn + (int)side) % 2, board->pad_index + 5);
    return 0;
}
