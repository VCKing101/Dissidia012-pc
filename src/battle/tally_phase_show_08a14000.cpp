#include "battle/agent28_types.h"

/* Tally phase states: show the round's scorers on the gauges and rig, then
 * after a pause hand the next turn on. */
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

extern RootHolder *D_089D6A10;
extern float D_08A1450C;
extern int func_089E99C0(int side, int fighter);
extern void func_08A020C8(void *mark);
extern void func_08A130AC(void *rig, int fighter, int unit);
extern void func_089F84E0(void *gauge);
extern int func_089F85B0(void *gauge, int fighter, int unit);
extern void func_089F854C(void *gauge);
extern void func_08889F28(int side, int cue);
extern void func_08889F70(int side, int cue);
extern void func_089DDCEC(void *root);

int func_08A14000(TallyPhase *phase)
{
    if (phase->shown == 1) {
        int current = (phase->wins + phase->losses) % 2;
        int next = (current + 1) % 2;
        int first = func_089E99C0(current, phase->winner);
        int second = func_089E99C0(next, phase->fighter);
        func_08A020C8(phase->mark);
        func_08A130AC(phase->rig, phase->fighter, second);
        func_089F84E0(&phase->gauges[0]);
        func_089F85B0(&phase->gauges[0], phase->winner, first);
        func_089F854C(&phase->gauges[1]);
        func_08889F28(next, phase->wins + 5);
        phase->state = 5;
        phase->time = 0.0f;
    }
    return 0;
}

int func_08A140FC(TallyPhase *phase, float dt)
{
    phase->time += dt;
    if (!(phase->time < D_08A1450C)) {
        void *root = D_089D6A10->root;
        if (root == 0) {
            return -1;
        }
        int next = (phase->wins + phase->losses + 1) % 2;
        int unit = func_089E99C0(next, phase->score);
        func_089F84E0(&phase->gauges[1]);
        func_089F85B0(&phase->gauges[1], phase->score, unit);
        func_08889F70(next, phase->wins + 5);
        func_089DDCEC(root);
        phase->state = 6;
        phase->time = 0.0f;
    }
    return 0;
}

}
