/* Paired bonus states: after the intro both prizes open; the prizes then
 * count the time left until the phase is ready. */
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

extern float D_08A1A404;
extern int D_08A1A408[];
extern int D_08A1A410[];
extern void func_08A01AE0(PairCue *cue, int kind, int sound);
extern void func_08A01BC8(PairCue *cue, int kind, int sound);
extern void func_08A191C0(PairPrize *prize);
extern void func_08A19038(PairPrize *prize, int shown, int *motions);
extern int func_08A19080(PairPrize *prize, int value);
extern float func_0889140C(void *rules);
extern int func_08A22DEC(float time);

int func_08A19D1C(PairBonusPhase *phase, float dt)
{
    phase->time += dt;
    if (!(phase->time < D_08A1A404)) {
        for (int i = 0; i < 2; i++) {
            func_08A01AE0(&phase->cues[i], 0, -1);
            func_08A191C0(&phase->prizes[i]);
            func_08A19038(&phase->prizes[i], 0, D_08A1A408);
        }
        phase->state = 1;
        phase->time = 0.0f;
    }
    return 0;
}

int func_08A19DDC(PairBonusPhase *phase)
{
    int seconds = func_08A22DEC(func_0889140C(phase->rules));
    for (int i = 0; i < 2; i++) {
        func_08A19080(&phase->prizes[i], seconds);
    }
    if (phase->ready == 1) {
        for (int i = 0; i < 2; i++) {
            func_08A01BC8(&phase->cues[i], 1, -1);
            func_08A19038(&phase->prizes[i], 1, D_08A1A410);
        }
        phase->state = 2;
        phase->time = 0.0f;
    }
    return 0;
}

}
