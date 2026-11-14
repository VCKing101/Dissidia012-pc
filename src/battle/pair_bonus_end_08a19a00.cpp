/* Ends the paired bonus phase's play state: closes both sides' cues and
 * prizes and tells the caller it is done. */
class PhaseBase {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void slot_028();
    virtual void slot_030();
    virtual void slot_038();
    virtual int is_active();
};

struct PairCue {
    unsigned char data[0x28];
};

struct PairPrize {
    unsigned char data[0x24];
};

struct PairMark {
    unsigned char data[0x24];
};

struct PairScorer {
    unsigned char data[0x118];
};

class PairBonusPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_0e[2];
    float time;
    PairCue cues[2];
    PairPrize prizes[2];
    PairMark marks[2];
    PairScorer scorers[2];
};

extern "C" {

extern void func_08A01C74(PairCue *cue, int kind);
extern int func_08A191E8(PairPrize *prize);

int func_08A19A00(PairBonusPhase *phase, unsigned char *done)
{
    if (!phase->is_active()) {
        return -1;
    }
    if (done == 0) {
        return -2;
    }
    if (phase->state == 2) {
        for (int i = 0; i < 2; i++) {
            func_08A01C74(&phase->cues[i], 3);
            func_08A191E8(&phase->prizes[i]);
        }
        phase->state = 6;
        phase->time = 0.0f;
        *done = 1;
    }
    return 0;
}

}
