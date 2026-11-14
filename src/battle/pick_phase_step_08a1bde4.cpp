/* Runs the handler of the pick phase's current state. */
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

class PickPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_0e[2];
    float time;
    int scorer;
    unsigned char cue[0x28];
    unsigned char prize[0x24];
    unsigned char mark[0x24];
    unsigned char pad_88[0x78];
    unsigned char gauge[0x38];
    unsigned char pad_138[0x34];
    int wins;
    int losses;
    void *rules;
    void *banner;
};

extern "C" {

extern int func_08A1BEEC(PickPhase *phase, float dt);
extern int func_08A1BF74(PickPhase *phase, float dt);
extern int func_08A1C004(PickPhase *phase, float dt);
extern int func_08A1C0A0(PickPhase *phase, float dt);
extern int func_08A1C1C4(PickPhase *phase, float dt);
extern int func_08A1C218(PickPhase *phase, float dt);
extern int func_08A1C264(PickPhase *phase, float dt);
extern int func_08A1C2A4(PickPhase *phase, float dt);
extern int func_08A1C2B4(PickPhase *phase, float dt);
extern int func_08A1C2FC(PickPhase *phase, float dt);
extern int func_08A1C348(PickPhase *phase, float dt);
extern int func_08A1C388(PickPhase *phase, float dt);

int func_08A1BDE4(PickPhase *phase, float x, float y)
{
    switch (phase->state) {
    case 0:
        return func_08A1BEEC(phase, x);
    case 1:
        return func_08A1BF74(phase, y);
    case 2:
        return func_08A1C004(phase, y);
    case 3:
        return func_08A1C0A0(phase, y);
    case 4:
        return func_08A1C1C4(phase, y);
    case 5:
        return func_08A1C218(phase, y);
    case 6:
        return func_08A1C264(phase, y);
    case 7:
        return func_08A1C2A4(phase, y);
    case 8:
        return func_08A1C2B4(phase, y);
    case 9:
        return func_08A1C2FC(phase, y);
    case 10:
        return func_08A1C348(phase, y);
    case 11:
        return func_08A1C388(phase, y);
    }
    return 0;
}

}
