/* The bonus phase: each frame it updates every part of its board; on start
 * it marks the side whose turn it is on the banner. */
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

extern int func_08A185E4(BonusPhase *phase, float x, float y);
extern void func_08A01948(void *cue);
extern void func_08A18F00(void *prize);
extern void func_08A01F94(void *mark);
extern void func_08A169DC(void *timer);
extern void func_08A008FC(void *board);
extern void func_08A08334(void *counter);
extern void func_089F5974(void *banner, int *marks, int flags);

int func_08A18110(BonusPhase *phase, float x, float y)
{
    if (!phase->is_active()) {
        return -1;
    }
    func_08A185E4(phase, x, y);
    func_08A01948(phase->cue);
    func_08A18F00(phase->prize);
    func_08A01F94(phase->mark);
    func_08A169DC(phase->timer);
    func_08A01F94(phase->second_mark);
    func_08A008FC(phase->board);
    func_08A08334(phase->counter);
    return 0;
}

}
