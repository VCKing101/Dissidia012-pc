/* Closes the match point phase once it reaches state 2: closes its cue and
 * progress displays, moves to state 3 and reports completion. */
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
    unsigned char cue[0x28];
    unsigned char progress[4];
};

extern "C" {

extern void func_08A01C74(void *cue, int kind);
extern void func_08A16C58(void *progress);

int func_08A10674(BattlePhase *phase, unsigned char *done)
{
    if (!phase->is_active()) {
        return -1;
    }
    if (done == 0) {
        return -2;
    }
    if (phase->state == 2) {
        func_08A01C74(phase->cue, 3);
        func_08A16C58(phase->progress);
        phase->state = 3;
        phase->time = 0.0f;
        *done = 1;
    }
    return 0;
}

}
