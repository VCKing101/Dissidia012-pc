/* Starts the round scorer: points its gauge at the fighter for the side
 * whose turn it is and keeps the round's score. */
struct RoundScorer {
    unsigned char open;
    unsigned char pad_01[3];
    int state;
    float time;
    unsigned char done;
    unsigned char pad_0d[3];
    int round;
    int score;
    unsigned char gauge[0xF4];
    int wins;
    int losses;
};

extern int func_089E99C0(int side, int fighter);
extern void func_089F84E0(void *gauge);
extern int func_089F85B0(void *gauge, int fighter, int unit);

int func_08A1785C(struct RoundScorer *scorer, int fighter, int round, float score)
{
    int side;
    int unit;

    if (!scorer->open) {
        return -1;
    }
    if (scorer->state != -1) {
        return -2;
    }
    side = (scorer->wins + scorer->losses) % 2;
    unit = func_089E99C0(side, fighter);
    func_089F84E0(scorer->gauge);
    func_089F85B0(scorer->gauge, fighter, unit);
    scorer->state = 0;
    scorer->time = 0.0f;
    scorer->done = 0;
    scorer->round = round;
    scorer->score = (int)score;
    return 0;
}
