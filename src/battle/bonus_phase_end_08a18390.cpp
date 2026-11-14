/* Ends the bonus phase's play state: closes the cue and the prize and tells
 * the caller it is done. */
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

class BonusPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_0e[2];
    float time;
    unsigned char cue[0x28];
    unsigned char prize[0x24];
    unsigned char mark[0x24];
    unsigned char timer[0x30];
    unsigned char second_mark[0x24];
    unsigned char board[0x3C];
    unsigned char counter[0x34];
    int wins;
    int losses;
    void *rules;
    void *banner;
};

extern "C" {

extern void func_08A01C74(void *cue, int kind);
extern void func_08A191E8(void *prize);

int func_08A18390(BonusPhase *phase, unsigned char *done)
{
    if (!phase->is_active()) {
        return -1;
    }
    if (done == 0) {
        return -2;
    }
    if (phase->state == 2) {
        func_08A01C74(phase->cue, 9);
        func_08A191E8(phase->prize);
        phase->state = 6;
        phase->time = 0.0f;
        *done = 1;
    }
    return 0;
}

}
