/* Per-frame update of the stage phase's displays. */
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

class StagePhase : public PhaseBase {
public:
    unsigned char pad_04[0x1C];
    unsigned char timer[0x2C];
    unsigned char gauge[0x38];
    unsigned char banner[0x48];
    unsigned char cue[0x28];
    unsigned char counter[0x14];
    unsigned char progress[0x30];
    unsigned char meter[0x34];
    unsigned char score[4];
};

extern "C" {

extern void func_089FF660(StagePhase *phase, float x, float y);
extern void func_08A10C88(void *timer);
extern int func_089F83AC(void *gauge);
extern void func_089F602C(void *banner, float x);
extern void func_08A01948(void *cue);
extern void func_08A110DC(void *counter);
extern void func_08A169DC(void *progress);
extern int func_089FE054(void *meter);
extern void func_089FC3E4(void *score);

int func_089FF2D0(StagePhase *phase, float x, float y)
{
    if (!phase->is_active()) {
        return -1;
    }
    func_089FF660(phase, x, y);
    func_08A10C88(phase->timer);
    func_089F83AC(phase->gauge);
    func_089F602C(phase->banner, x);
    func_08A01948(phase->cue);
    func_08A110DC(phase->counter);
    func_08A169DC(phase->progress);
    func_089FE054(phase->meter);
    func_089FC3E4(phase->score);
    return 0;
}

}
