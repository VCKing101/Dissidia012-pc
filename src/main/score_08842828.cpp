struct ScoreObj {
    char pad_00[8];
    unsigned char flags;

    static bool hasFlag8(ScoreObj *o) { return (o->flags & 8) != 0; }
};

extern "C" {

int func_08842828(ScoreObj *o, bool primary, bool bonus)
{
    int score = 0;

    if (primary == true) {
        if (o->flags & 1) {
            score = 10000;
        }
        if (bonus == true) {
            score += 30000;
        }
    }
    if (ScoreObj::hasFlag8(o) == true) {
        score += 50000;
    }
    return score;
}

}
