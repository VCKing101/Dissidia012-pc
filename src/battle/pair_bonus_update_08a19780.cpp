/* The paired bonus phase: each frame it updates both sides' cue, prize, mark
 * and scorer. */
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

extern int func_08A19C54(PairBonusPhase *phase, float x, float y);
extern void func_08A01948(PairCue *cue);
extern void func_08A18F00(PairPrize *prize);
extern void func_08A01F94(PairMark *mark);
extern void func_08A17808(PairScorer *scorer, float dt);

int func_08A19780(PairBonusPhase *phase, float x, float y)
{
    if (!phase->is_active()) {
        return -1;
    }
    func_08A19C54(phase, x, y);
    for (int i = 0; i < 2; i++) {
        func_08A01948(&phase->cues[i]);
        func_08A18F00(&phase->prizes[i]);
        func_08A01F94(&phase->marks[i]);
        func_08A17808(&phase->scorers[i], x);
    }
    return 0;
}

}
