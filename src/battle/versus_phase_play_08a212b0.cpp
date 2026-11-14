/* Versus play state: on a win shows the Brave panel and counter, and keeps
 * the clock on the round time; when the round is over it closes them. */
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

struct VersusResult {
    unsigned char pad_00[8];
    float time;
    unsigned char pad_0c[0x18];
    unsigned char won;
    unsigned char pad_25[7];
};

class VersusPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char pad_0d[3];
    float time;
    unsigned char cue[0x28];
    unsigned char clock[0x30];
    unsigned char mark[0x24];
    unsigned char panel[0x38];
    unsigned char digits[0x24];
    int wins;
    int losses;
    void *rules;
    void *banner;
};

extern "C" {

extern int func_08A22E2C(VersusResult *result, int wins, int losses, int kind);
extern void func_08A020C8(void *mark);
extern int func_08A20328(void *panel);
extern int func_08A20A00(void *digits);
extern float func_08891348(void *rules);
extern void func_08A16CC4(void *clock, float rate);
extern void func_08A01C74(void *cue, int kind);
extern void func_08A16C58(void *clock);
extern void func_08A203E4(void *panel);

int func_08A212B0(VersusPhase *phase)
{
    VersusResult result;
    if (func_08A22E2C(&result, phase->wins, phase->losses, 0) == 0) {
        if (result.won == 1) {
            func_08A020C8(phase->mark);
            func_08A20328(phase->panel);
            func_08A20A00(phase->digits);
        }
        float length = func_08891348(phase->rules);
        if (length != 0.0f) {
            func_08A16CC4(phase->clock, result.time / length);
        }
    } else {
        func_08A01C74(phase->cue, 3);
        func_08A16C58(phase->clock);
        func_08A203E4(phase->panel);
        phase->state = 3;
    }
    return 0;
}

}
