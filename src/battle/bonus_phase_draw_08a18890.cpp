/* Draws the bonus: on a hit the board pays out and the side whose turn it
 * is lights up; on a miss the phase moves to its miss state. */
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

extern int func_08891D84(void *rules, unsigned char *decider, int a, int b, int c, int d);
extern void func_08A01BC8(void *cue, int kind, int sound);
extern void func_08A020C8(void *mark);
extern void func_08A191E8(void *prize);
extern void func_08A00ADC(void *board);
extern void func_089F5994(void *banner, int *marks, int flags, float delay);

int func_08A18890(BonusPhase *phase)
{
    if (phase->shown == 1) {
        unsigned char missed = 0;
        if (func_08891D84(phase->rules, &missed, 0, 0, 0, 0) != 0) {
            return -1;
        }
        func_08A01BC8(phase->cue, 6, -1);
        func_08A020C8(phase->mark);
        func_08A191E8(phase->prize);
        int marks[3];
        for (int i = 0; i < 3; i++) {
            marks[i] = 2;
        }
        int current = (phase->wins + phase->losses) % 2;
        if (!missed) {
            func_08A00ADC(phase->board);
            marks[current] = 1;
            phase->state = 4;
        } else {
            marks[current] = 0;
            phase->state = 7;
        }
        func_089F5994(phase->banner, marks, 0, 0.0f);
    }
    return 0;
}

}
