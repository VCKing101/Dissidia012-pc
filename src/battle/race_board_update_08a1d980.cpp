/* The race board phase: each frame it updates every part of its board. */
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

class RaceBoardPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_0e[2];
    float time;
    unsigned char pad_14[0xC];
    unsigned char bound_timer[0x2C];
    unsigned char gauge[0x38];
    unsigned char trail[0x48];
    unsigned char cue[0x28];
    unsigned char timer[0x30];
    unsigned char meter[0x34];
    unsigned char score[0x34];
};

extern "C" {

extern int func_08A1DD08(RaceBoardPhase *phase, float x, float y);
extern void func_08A10C88(void *timer);
extern void func_089F83AC(void *gauge);
extern void func_089F602C(void *trail, float x);
extern void func_08A01948(void *cue);
extern void func_08A169DC(void *timer);
extern void func_089FE054(void *meter);
extern void func_089FC3E4(void *score);

int func_08A1D980(RaceBoardPhase *phase, float x, float y)
{
    if (!phase->is_active()) {
        return -1;
    }
    func_08A1DD08(phase, x, y);
    func_08A10C88(phase->bound_timer);
    func_089F83AC(phase->gauge);
    func_089F602C(phase->trail, x);
    func_08A01948(phase->cue);
    func_08A169DC(phase->timer);
    func_089FE054(phase->meter);
    func_089FC3E4(phase->score);
    return 0;
}

}
