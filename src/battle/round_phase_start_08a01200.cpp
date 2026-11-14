/* Starts the duel round: asks whether the round is a decider, marks the
 * banner for the side whose turn it is and picks the opening state. */
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

class RoundPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char pad_0d[3];
    float time;
    unsigned char pad_14[0xEC];
    int wins;
    int losses;
    void *rules;
    void *banner;
};

extern "C" {

extern int func_08891D84(void *rules, unsigned char *decider, int a, int b, int c, int d);
extern void func_089F5974(void *banner, int *marks, int flags);

int func_08A01200(RoundPhase *phase)
{
    if (!phase->is_active()) {
        return -1;
    }
    if (phase->state != -1) {
        return -2;
    }
    unsigned char decider;
    int marks[3];
    decider = 0;
    if (func_08891D84(phase->rules, &decider, 0, 0, 0, 0) != 0) {
        return -3;
    }
    for (int i = 0; i < 3; i++) {
        marks[i] = 2;
    }
    int current = (phase->wins + phase->losses) % 2;
    if (decider == 0) {
        marks[current] = 1;
        phase->state = 0;
    } else {
        marks[current] = 0;
        phase->state = 4;
    }
    func_089F5974(phase->banner, marks, 0);
    phase->ready = 0;
    phase->time = 0.0f;
    return 0;
}

}
