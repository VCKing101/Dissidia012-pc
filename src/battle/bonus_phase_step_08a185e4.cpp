/* Runs the handler of the bonus phase's current state. */
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

class BonusPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_0e[2];
    float time;
    unsigned char cue[0x28];
    unsigned char prize[0x24];
    unsigned char mark[0x24];
    unsigned char timer[0x30];
    unsigned char second_mark[0x24];
    unsigned char board[0x3C];
    unsigned char counter[0x34];
    int wins;
    int losses;
    void *rules;
    void *banner;
};

extern "C" {

extern int func_08A186DC(BonusPhase *phase, float dt);
extern int func_08A18764(BonusPhase *phase, float dt);
extern int func_08A187F4(BonusPhase *phase, float dt);
extern int func_08A18890(BonusPhase *phase, float dt);
extern int func_08A189B0(BonusPhase *phase, float dt);
extern int func_08A189F0(BonusPhase *phase, float dt);
extern int func_08A18AAC(BonusPhase *phase, float dt);
extern int func_08A18AEC(BonusPhase *phase, float dt);
extern int func_08A18B34(BonusPhase *phase, float dt);
extern int func_08A18B88(BonusPhase *phase, float dt);
extern int func_08A18BC8(BonusPhase *phase, float dt);

int func_08A185E4(BonusPhase *phase, float x, float y)
{
    switch (phase->state) {
    case 0:
        return func_08A186DC(phase, x);
    case 1:
        return func_08A18764(phase, y);
    case 2:
        return func_08A187F4(phase, y);
    case 3:
        return func_08A18890(phase, y);
    case 4:
        return func_08A189B0(phase, y);
    case 5:
        return func_08A189F0(phase, y);
    case 6:
        return func_08A18AAC(phase, y);
    case 7:
        return func_08A18AEC(phase, y);
    case 8:
        return func_08A18B34(phase, y);
    case 9:
        return func_08A18B88(phase, y);
    case 10:
        return func_08A18BC8(phase, y);
    }
    return 0;
}

}
