/* Runs the handler of the paired bonus phase's current state. */
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
    unsigned char pad_324[4];
    void *rules;
};

extern "C" {

extern int func_08A19D1C(PairBonusPhase *phase, float dt);
extern int func_08A19DDC(PairBonusPhase *phase, float dt);
extern int func_08A19EC0(PairBonusPhase *phase, float dt);
extern int func_08A19F90(PairBonusPhase *phase, float dt);
extern int func_08A1A16C(PairBonusPhase *phase, float dt);
extern int func_08A1A210(PairBonusPhase *phase, float dt);
extern int func_08A1A2B8(PairBonusPhase *phase, float dt);
extern int func_08A1A33C(PairBonusPhase *phase, float dt);

int func_08A19C54(PairBonusPhase *phase, float x, float y)
{
    switch (phase->state) {
    case 0:
        return func_08A19D1C(phase, x);
    case 1:
        return func_08A19DDC(phase, y);
    case 2:
        return func_08A19EC0(phase, y);
    case 3:
        return func_08A19F90(phase, y);
    case 4:
        return func_08A1A16C(phase, y);
    case 5:
        return func_08A1A210(phase, y);
    case 6:
        return func_08A1A2B8(phase, y);
    case 7:
        return func_08A1A33C(phase, y);
    }
    return 0;
}

}
