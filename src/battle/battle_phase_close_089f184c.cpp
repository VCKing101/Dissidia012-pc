/* Closes the round phase once it reaches state 2: closes the cue, moves to
 * state 3 and reports completion through the caller's flag. */
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

class BattlePhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char pad_0c[4];
    float time;
    unsigned char cue[4];
};

extern "C" {

extern void func_089F1F24(void *cue);

int func_089F184C(BattlePhase *phase, unsigned char *done)
{
    if (!phase->is_active()) {
        return -1;
    }
    if (done == 0) {
        return -2;
    }
    if (phase->state == 2) {
        func_089F1F24(phase->cue);
        phase->state = 3;
        phase->time = 0.0f;
        *done = 1;
    }
    return 0;
}

}
