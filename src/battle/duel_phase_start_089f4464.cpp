/* Starts the duel phase: shows the banner with both sides marked and clears
 * the per-side state. */
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

class DuelPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_0e[2];
    float time;
    unsigned char done[2];
    unsigned char pad_16[2];
    int scores[2];
    unsigned char pad_20[0x3B4];
    void *banner;
};

extern "C" {

extern void func_089F5974(void *banner, int *marks, int flags);

int func_089F4464(DuelPhase *phase)
{
    if (!phase->is_active()) {
        return -1;
    }
    if (phase->state != -1) {
        return -2;
    }
    int marks[3];
    for (int i = 0; i < 3; i++) {
        marks[i] = 1;
    }
    marks[2] = 2;
    func_089F5974(phase->banner, marks, 0);
    for (int side = 0; side < 2; side++) {
        phase->done[side] = 0;
        phase->scores[side] = 0;
    }
    phase->state = 0;
    phase->ready = 0;
    phase->shown = 0;
    phase->time = 0.0f;
    return 0;
}

}
