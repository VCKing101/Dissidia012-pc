/* Starts the card phase: resets the cue and marks the current side and the
 * shared slot on the banner. */
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

class CardPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char pad_0d[3];
    float time;
    unsigned char pad_14[0x58];
    int wins;
    int losses;
    void *cue;
    void *banner;
};

extern "C" {

extern void func_08891D84(void *cue, int a, int b, int c, int d, int e);
extern void func_089F5974(void *banner, int *marks, int flags);

int func_08A2A148(CardPhase *phase)
{
    if (!phase->is_active()) {
        return -1;
    }
    if (phase->state != -1) {
        return -2;
    }
    func_08891D84(phase->cue, 0, 0, 0, 0, 0);
    int marks[3];
    for (int i = 0; i < 3; i++) {
        marks[i] = 2;
    }
    int current = (phase->wins + phase->losses) % 2;
    int active = 1;
    marks[current] = active;
    marks[2] = active;
    func_089F5974(phase->banner, marks, 0);
    phase->state = 0;
    phase->ready = 0;
    phase->time = 0.0f;
    return 0;
}

}
