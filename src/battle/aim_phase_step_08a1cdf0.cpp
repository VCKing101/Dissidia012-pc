/* Runs the handler of the aim phase's current state. */
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

class AimPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_0e[2];
    float time;
    unsigned char cue[0x28];
    unsigned char aim_cue[0x28];
    unsigned char target[0x28];
    unsigned char gauge[0x38];
    unsigned char timer[0x2C];
    unsigned char pad_f0[0x34];
    int wins;
    int losses;
    void *rules;
    void *banner;
};

extern "C" {

extern int func_08A1CED8(AimPhase *phase, float dt);
extern int func_08A1CF38(AimPhase *phase, float dt);
extern int func_08A1CFE4(AimPhase *phase, float dt);
extern int func_08A1D0AC(AimPhase *phase, float dt);
extern int func_08A1D224(AimPhase *phase, float dt);
extern int func_08A1D27C(AimPhase *phase, float dt);
extern int func_08A1D2D0(AimPhase *phase, float dt);
extern int func_08A1D310(AimPhase *phase, float dt);
extern int func_08A1D360(AimPhase *phase, float dt);
extern int func_08A1D3B4(AimPhase *phase, float dt);

int func_08A1CDF0(AimPhase *phase, float x, float y)
{
    switch (phase->state) {
    case 0:
        return func_08A1CED8(phase, x);
    case 1:
        return func_08A1CF38(phase, y);
    case 2:
        return func_08A1CFE4(phase, y);
    case 3:
        return func_08A1D0AC(phase, y);
    case 4:
        return func_08A1D224(phase, x);
    case 5:
        return func_08A1D27C(phase, y);
    case 6:
        return func_08A1D2D0(phase, y);
    case 7:
        return func_08A1D310(phase, x);
    case 8:
        return func_08A1D360(phase, y);
    case 9:
        return func_08A1D3B4(phase, y);
    }
    return 0;
}

}
