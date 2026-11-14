/* Runs the handler of the guard phase's current state. */
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

class GuardPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_0e[2];
    float time;
    int winner;
    int loser;
    unsigned char cue[0x28];
    unsigned char timer[0x30];
    unsigned char mark[0x24];
    unsigned char bound_timer[0x2C];
    unsigned char gauge[0x38];
    unsigned char trail[0x48];
    unsigned char counter[0x34];
    int wins;
    int losses;
    void *rules;
    void *banner;
};

extern "C" {

extern int func_08A15038(GuardPhase *phase, float dt);
extern int func_08A15098(GuardPhase *phase, float dt);
extern int func_08A150E8(GuardPhase *phase, float dt);
extern int func_08A15144(GuardPhase *phase, float dt);
extern int func_08A15184(GuardPhase *phase, float dt);
extern int func_08A15228(GuardPhase *phase, float dt);
extern int func_08A15298(GuardPhase *phase, float dt);
extern int func_08A152D0(GuardPhase *phase, float dt);
extern int func_08A1533C(GuardPhase *phase, float dt);
extern int func_08A1537C(GuardPhase *phase, float dt);
extern int func_08A153C4(GuardPhase *phase, float dt);
extern int func_08A15404(GuardPhase *phase, float dt);

int func_08A14F38(GuardPhase *phase, float x, float y)
{
    switch (phase->state) {
    case 0:
        return func_08A15038(phase, x);
    case 1:
        return func_08A15098(phase, x);
    case 2:
        return func_08A150E8(phase, x);
    case 3:
        return func_08A15144(phase, x);
    case 4:
        return func_08A15184(phase, x);
    case 5:
        return func_08A15228(phase, x);
    case 6:
        return func_08A15298(phase, x);
    case 7:
        return func_08A152D0(phase, x);
    case 8:
        return func_08A1533C(phase, x);
    case 9:
        return func_08A1537C(phase, x);
    case 10:
        return func_08A153C4(phase, x);
    case 11:
        return func_08A15404(phase, x);
    }
    return 0;
}

}
