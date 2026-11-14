/* Per-frame update of the duel phase's displays. */
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

struct DuelGauge {
    unsigned char data[0x38];
};

struct DuelPartSet {
    unsigned char data[0x140];
};

struct DuelBadge {
    unsigned char data[0x34];
};

class DuelPhase : public PhaseBase {
public:
    unsigned char pad_04[0x1C];
    unsigned char widget[0x1C];
    unsigned char timer[0x2C];
    DuelGauge gauges[2];
    unsigned char pad_d8[8];
    DuelPartSet parts[2];
    DuelBadge badges[2];
};

extern "C" {

extern void func_089F4800(DuelPhase *phase, float x, float y);
extern int func_089F39E4(void *widget);
extern void func_08A10C88(void *timer);
extern void func_089F83AC(DuelGauge *gauge);
extern int func_089F35C0(DuelPartSet *parts);
extern void func_08A08334(DuelBadge *badge);

int func_089F4380(DuelPhase *phase, float x, float y)
{
    if (!phase->is_active()) {
        return -1;
    }
    func_089F4800(phase, x, y);
    func_089F39E4(phase->widget);
    func_08A10C88(phase->timer);
    for (int i = 0; i < 2; i++) {
        func_089F83AC(&phase->gauges[i]);
        func_089F35C0(&phase->parts[i]);
        func_08A08334(&phase->badges[i]);
    }
    return 0;
}

}
