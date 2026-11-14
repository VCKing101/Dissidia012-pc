/* Draws the pick for the fighter whose turn it is: on a hit the gauge
 * points at the fighter; on a miss the phase moves to its miss state. */
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

struct FighterInfo {
    unsigned char pad_00[0x14];
    short id;
};

struct FighterUnit {
    unsigned char pad_000[0x52C];
    FighterInfo *info;
};

struct PrizeDraw {
    int points;
    unsigned char pad_04[0x14];
    int bonus;

    PrizeDraw() : bonus(0) {}
};

class PickPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_0e[2];
    float time;
    int scorer;
    unsigned char cue[0x28];
    unsigned char prize[0x24];
    unsigned char mark[0x24];
    unsigned char pad_88[0x78];
    unsigned char gauge[0x38];
    unsigned char pad_138[0x34];
    int wins;
    int losses;
    void *rules;
    void *banner;
};

extern "C" {

extern FighterUnit *func_08A22C78(int side);
extern int func_089E99C0(int side, int fighter);
extern int func_08891D84(void *rules, unsigned char *missed, PrizeDraw *draw, int a, int b, int c);
extern void func_08A020C8(void *mark);
extern int func_08A191E8(void *prize);
extern void func_089F84E0(void *gauge);
extern int func_089F85B0(void *gauge, int fighter, int unit);

int func_08A1C0A0(PickPhase *phase)
{
    if (phase->shown == 1) {
        int side = (phase->wins + phase->losses) % 2;
        FighterUnit *unit = func_08A22C78(side);
        if (unit == 0) {
            return -1;
        }
        int fighter = unit->info->id;
        unsigned char missed = 0;
        PrizeDraw draw;
        if (func_08891D84(phase->rules, &missed, &draw, 0, 0, 0) != 0) {
            return -2;
        }
        func_08A020C8(phase->mark);
        func_08A191E8(phase->prize);
        if (!missed) {
            int slot = func_089E99C0(side, fighter);
            func_089F84E0(phase->gauge);
            func_089F85B0(phase->gauge, fighter, slot);
            phase->state = 4;
            phase->scorer = fighter;
        } else {
            phase->state = 8;
        }
    }
    return 0;
}

}
