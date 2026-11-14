/* Finishes the duel phase once it reaches state 2: moves to state 7 and
 * reports completion through the caller's flag. */
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
};

extern "C" {

int func_089F4618(BattlePhase *phase, unsigned char *done)
{
    if (!phase->is_active()) {
        return -1;
    }
    if (done == 0) {
        return -2;
    }
    if (phase->state == 2) {
        phase->state = 7;
        phase->time = 0.0f;
        *done = 1;
    }
    return 0;
}

}
