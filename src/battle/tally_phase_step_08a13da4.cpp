/* Runs the handler of the tally phase's current state. */
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

struct TallyGauge {
    unsigned char data[0x38];
};

class TallyPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_0e[2];
    float time;
    int winner;
    int fighter;
    int loser;
    int score;
    unsigned char cue[0x28];
    unsigned char timer[0x30];
    unsigned char mark[0x24];
    TallyGauge gauges[2];
    unsigned char rig[0x68];
    unsigned char trail[0x48];
    unsigned char counter[0x34];
    int wins;
    int losses;
    void *rules;
    void *banner;
};

extern "C" {

extern int func_08A13EB4(TallyPhase *phase, float dt);
extern int func_08A13F14(TallyPhase *phase, float dt);
extern int func_08A13F64(TallyPhase *phase, float dt);
extern int func_08A13FC0(TallyPhase *phase, float dt);
extern int func_08A14000(TallyPhase *phase, float dt);
extern int func_08A140FC(TallyPhase *phase, float dt);
extern int func_08A141EC(TallyPhase *phase, float dt);
extern int func_08A1426C(TallyPhase *phase, float dt);
extern int func_08A142A4(TallyPhase *phase, float dt);
extern int func_08A14338(TallyPhase *phase, float dt);
extern int func_08A14380(TallyPhase *phase, float dt);
extern int func_08A143D0(TallyPhase *phase, float dt);
extern int func_08A14410(TallyPhase *phase, float dt);

int func_08A13DA4(TallyPhase *phase, float x, float y)
{
    switch (phase->state) {
    case 0:
        return func_08A13EB4(phase, x);
    case 1:
        return func_08A13F14(phase, x);
    case 2:
        return func_08A13F64(phase, x);
    case 3:
        return func_08A13FC0(phase, x);
    case 4:
        return func_08A14000(phase, x);
    case 5:
        return func_08A140FC(phase, x);
    case 6:
        return func_08A141EC(phase, x);
    case 7:
        return func_08A1426C(phase, x);
    case 8:
        return func_08A142A4(phase, x);
    case 9:
        return func_08A14338(phase, x);
    case 10:
        return func_08A14380(phase, x);
    case 11:
        return func_08A143D0(phase, x);
    case 12:
        return func_08A14410(phase, x);
    }
    return 0;
}

}
