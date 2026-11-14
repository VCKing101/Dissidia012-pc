/* Runs the handler of the bout phase's current state. */
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

class BoutPhase : public PhaseBase {
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
    unsigned char first_timer[0x2C];
    unsigned char second_timer[0x2C];
    unsigned char gauge[0x40];
    unsigned char parts[0x150];
    unsigned char counter[0x34];
    int wins;
    int losses;
    void *rules;
    void *banner;
};

extern "C" {

extern int func_08A1255C(BoutPhase *phase);
extern int func_08A125BC(BoutPhase *phase);
extern int func_08A1260C(BoutPhase *phase);
extern int func_08A12668(BoutPhase *phase);
extern int func_08A126A8(BoutPhase *phase);
extern int func_08A12744(BoutPhase *phase);
extern int func_08A127A4(BoutPhase *phase);
extern int func_08A12870(BoutPhase *phase);
extern int func_08A128D8(BoutPhase *phase);
extern int func_08A12944(BoutPhase *phase);
extern int func_08A1298C(BoutPhase *phase);
extern int func_08A129EC(BoutPhase *phase);
extern int func_08A12A44(BoutPhase *phase);

int func_08A1244C(BoutPhase *phase, float x, float y)
{
    switch (phase->state) {
    case 0:
        return func_08A1255C(phase);
    case 1:
        return func_08A125BC(phase);
    case 2:
        return func_08A1260C(phase);
    case 3:
        return func_08A12668(phase);
    case 4:
        return func_08A126A8(phase);
    case 5:
        return func_08A12744(phase);
    case 6:
        return func_08A127A4(phase);
    case 7:
        return func_08A12870(phase);
    case 8:
        return func_08A128D8(phase);
    case 9:
        return func_08A12944(phase);
    case 10:
        return func_08A1298C(phase);
    case 11:
        return func_08A129EC(phase);
    case 12:
        return func_08A12A44(phase);
    }
    return 0;
}

}
