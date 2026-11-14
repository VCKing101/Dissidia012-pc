/* Starts the look phase: shows each banner slot's look with no slot marked. */
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

class LookPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char pad_0d[3];
    float time;
    unsigned char pad_14[0x3C];
    void *banner;
};

extern "C" {

extern int func_089E978C(int slot);
extern void func_089F5974(void *banner, int *marks, int *looks);

int func_08A288A8(LookPhase *phase)
{
    if (!phase->is_active()) {
        return -1;
    }
    if (phase->state != -1) {
        return -2;
    }
    int looks[3];
    for (int i = 0; i < 3; i++) {
        looks[i] = func_089E978C(i);
    }
    int marks[3];
    for (int i = 0; i < 3; i++) {
        marks[i] = 0;
    }
    func_089F5974(phase->banner, marks, looks);
    phase->ready = 0;
    phase->time = 0.0f;
    phase->state = 0;
    return 0;
}

}
