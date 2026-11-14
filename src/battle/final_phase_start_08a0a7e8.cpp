/* Starts the final phase: marks the side whose turn it is when the rules
 * let it play on, otherwise ends the rules timer and skips to state 3. */
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

class FinalPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char pad_0d[3];
    float time;
    unsigned char pad_14[0x998];
    int wins;
    int losses;
    void *rules;
    void *match;
    void *banner;
};

extern "C" {

extern int func_08891D84(void *rules, unsigned char *decider, int a, int b, int c, int d);
extern void *func_08A22C78(int turn);
extern int func_0889120C(void *match);
extern int func_08891944(void *match);
extern void func_08890E14(void *rules);
extern void func_089F5974(void *banner, int *marks, int flags);

int func_08A0A7E8(FinalPhase *phase)
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
    if (func_08A22C78((phase->wins + phase->losses) % 2) == 0) {
        return -3;
    }
    int current = (phase->wins + phase->losses) % 2;
    if (func_0889120C(phase->match) == 1 && func_08891944(phase->match) == 0) {
        marks[current] = 1;
        phase->state = 0;
    } else {
        marks[current] = 0;
        phase->state = 3;
        func_08890E14(phase->rules);
    }
    func_089F5974(phase->banner, marks, 0);
    phase->ready = 0;
    phase->time = 0.0f;
    return 0;
}

}
