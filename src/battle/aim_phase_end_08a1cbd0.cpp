/* Ends the aim phase's play state: closes its three cues and tells the
 * caller it is done. */
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

class AimPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_0e[2];
    float time;
    unsigned char cue[0x28];
    unsigned char aim_cue[0x28];
    unsigned char target[0x28];
    unsigned char gauge[0x38];
    unsigned char timer[0x2C];
    unsigned char pad_f0[0x34];
    int wins;
    int losses;
    void *rules;
    void *banner;
};

extern "C" {

extern void func_08A01C74(void *cue, int kind);
extern void func_08A01CF4(void *cue);

int func_08A1CBD0(AimPhase *phase, unsigned char *done)
{
    if (!phase->is_active()) {
        return -1;
    }
    if (done == 0) {
        return -2;
    }
    if (phase->state == 2) {
        func_08A01C74(phase->cue, 4);
        func_08A01CF4(phase->target);
        func_08A01CF4(phase->aim_cue);
        phase->state = 6;
        phase->time = 0.0f;
        *done = 1;
    }
    return 0;
}

}
