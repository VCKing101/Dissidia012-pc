/* Starts the pick phase: marks the side whose turn it is on the banner and
 * clears the last scorer. */
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

extern void func_089F5974(void *banner, int *marks, int flags);

int func_08A1BA10(PickPhase *phase)
{
    if (!phase->is_active()) {
        return -1;
    }
    if (phase->state != -1) {
        return -2;
    }
    int marks[3];
    for (int i = 0; i < 3; i++) {
        marks[i] = 2;
    }
    int current = (phase->wins + phase->losses) % 2;
    int active = 1;
    marks[current] = active;
    func_089F5974(phase->banner, marks, 0);
    phase->state = 0;
    phase->ready = 0;
    phase->shown = 0;
    phase->time = 0.0f;
    phase->scorer = 0;
    return 0;
}

}
