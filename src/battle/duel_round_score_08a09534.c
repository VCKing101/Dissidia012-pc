/* After the score delay, shows the score of the fighter whose turn it was
 * and moves to state 1. */
typedef struct ScoreInfo {
    unsigned char pad_00[0x14];
    short score;
} ScoreInfo;

typedef struct ScoreFighter {
    unsigned char pad_000[0x52C];
    ScoreInfo *info;
} ScoreFighter;

typedef struct DuelRoundData {
    unsigned char pad_00[8];
    int state;
    unsigned char pad_0c[4];
    float time;
    unsigned char pad_14[0x88];
    unsigned char marker[0x40];
    int wins;
    int losses;
} DuelRoundData;

extern float D_08A09860;
extern ScoreFighter *func_08A22C78(int turn);
extern int func_089E99C0(int turn, int score);
extern int func_08A087A8(void *marker, int kind, unsigned char flag);

int func_08A09534(DuelRoundData *round, float step)
{
    round->time += step;
    if (round->time >= D_08A09860) {
        int turn = (round->wins + round->losses) % 2;
        ScoreFighter *fighter = func_08A22C78(turn);
        int score;
        int won;

        if (fighter == 0) {
            return -1;
        }
        score = fighter->info->score;
        won = func_089E99C0(turn, score) != 0;
        func_08A087A8(round->marker, score, won);
        round->state = 1;
    }
    return 0;
}
