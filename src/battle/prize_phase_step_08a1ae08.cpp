/* Runs the handler of the prize phase's current state. */
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

class PrizePhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_0e[2];
    float time;
    int scorer;
    int points;
    unsigned char cue[0x28];
    unsigned char prize[0x24];
    unsigned char mark[0x24];
    unsigned char timer[0x2C];
    unsigned char gauge[0x38];
    unsigned char pad_f0[0x7C];
    int wins;
    int losses;
    void *rules;
    void *banner;
};

extern "C" {

extern int func_08A1AF10(PrizePhase *phase, float dt);
extern int func_08A1AF98(PrizePhase *phase, float dt);
extern int func_08A1B028(PrizePhase *phase, float dt);
extern int func_08A1B0C4(PrizePhase *phase, float dt);
extern int func_08A1B1F0(PrizePhase *phase, float dt);
extern int func_08A1B250(PrizePhase *phase, float dt);
extern int func_08A1B29C(PrizePhase *phase, float dt);
extern int func_08A1B2DC(PrizePhase *phase, float dt);
extern int func_08A1B310(PrizePhase *phase, float dt);
extern int func_08A1B358(PrizePhase *phase, float dt);
extern int func_08A1B3A4(PrizePhase *phase, float dt);
extern int func_08A1B3E4(PrizePhase *phase, float dt);

int func_08A1AE08(PrizePhase *phase, float x, float y)
{
    switch (phase->state) {
    case 0:
        return func_08A1AF10(phase, x);
    case 1:
        return func_08A1AF98(phase, y);
    case 2:
        return func_08A1B028(phase, y);
    case 3:
        return func_08A1B0C4(phase, y);
    case 4:
        return func_08A1B1F0(phase, y);
    case 5:
        return func_08A1B250(phase, y);
    case 6:
        return func_08A1B29C(phase, y);
    case 7:
        return func_08A1B2DC(phase, y);
    case 8:
        return func_08A1B310(phase, y);
    case 9:
        return func_08A1B358(phase, y);
    case 10:
        return func_08A1B3A4(phase, y);
    case 11:
        return func_08A1B3E4(phase, y);
    }
    return 0;
}

}
