/* The tally phase: waits for the round result; on a win it points the gauge
 * at the fighter who scored, then hands the finish time to the timer. */
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

struct TallyResult {
    unsigned char pad_00[8];
    float time;
    int winner;
    int loser;
    int fighter;
    unsigned char pad_18[4];
    int score;
    unsigned char pad_20[4];
    unsigned char won;
    unsigned char lost;
    unsigned char pad_26[6];
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

extern int func_08A22E2C(TallyResult *result, int wins, int losses, int kind);
extern float func_08891348(void *rules);
extern void func_08A16CC4(void *timer, float rate);
extern int func_089E99C0(int side, int fighter);
extern void func_089F84E0(void *gauge);
extern int func_089F85B0(void *gauge, int fighter, int unit);
extern int func_08A13DA4(TallyPhase *phase, float x, float y);
extern void func_08A01948(void *cue);
extern void func_08A169DC(void *timer);
extern void func_08A01F94(void *mark);
extern void func_089F83AC(void *gauge);
extern void func_08A12E94(void *rig);
extern void func_089F602C(void *trail, float x);
extern void func_08A08334(void *counter);
extern int func_08891D84(void *rules, unsigned char *decider, int a, int b, int c, int d);
extern void func_089F5974(void *banner, int *marks, int flags);

int func_08A13A24(TallyPhase *phase)
{
    if (!phase->is_active()) {
        return -1;
    }
    if (phase->state != -1) {
        return -2;
    }
    func_08891D84(phase->rules, 0, 0, 0, 0, 0);
    int marks[3];
    for (int i = 0; i < 3; i++) {
        marks[i] = 2;
    }
    int current = (phase->wins + phase->losses) % 2;
    int next = (current + 1) % 2;
    int active = 1;
    marks[current] = active;
    marks[next] = active;
    func_089F5974(phase->banner, marks, 0);
    phase->state = 0;
    phase->ready = 0;
    phase->shown = 0;
    phase->time = 0.0f;
    return 0;
}

}
