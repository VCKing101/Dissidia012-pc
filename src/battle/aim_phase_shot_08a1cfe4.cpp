/* Aim phase states: the aim fills up over the round time and then the shot
 * is drawn; a hit points the gauge at the fighter, and a doubled hit also
 * plays the bonus sound. */
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

struct AimDraw {
    int points;
    unsigned char pad_04[0x14];
    int flags;

    AimDraw() : flags(0) {}

    static bool doubled(AimDraw *draw) { return (draw->flags & 4) != 0; }
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

extern float D_08A1D4B4;
extern void *D_08A62280;
extern float func_0889140C(void *rules);
extern void *func_08A01AC0(void *cue);
extern void func_088D9AEC(void *model, float speed);
extern FighterUnit *func_08A22C78(int side);
extern int func_089E99C0(int side, int fighter);
extern int func_08891D84(void *rules, unsigned char *missed, AimDraw *draw, int a, int b, int c);
extern void func_089F84E0(void *gauge);
extern int func_089F85B0(void *gauge, int fighter, int unit);
extern void func_08A62020(void *sounds, int id, float delay);
extern void func_08A10DB0(void *timer);
extern void func_08A10F08(void *timer);
extern void func_08A01CF4(void *cue);

int func_08A1CFE4(AimPhase *phase, float dt)
{
    float limit = func_0889140C(phase->rules);
    phase->time += dt;
    if (phase->time > limit) {
        phase->time = limit;
    }
    void *model = func_08A01AC0(phase->target);
    if (limit != 0.0f && model != 0) {
        func_088D9AEC(model, D_08A1D4B4 * phase->time / limit);
    }
    if (!(phase->time < limit)) {
        phase->state = 3;
        phase->time = 0.0f;
    }
    return 0;
}

int func_08A1D0AC(AimPhase *phase)
{
    if (phase->shown == 1) {
        int side = (phase->wins + phase->losses) % 2;
        FighterUnit *unit = func_08A22C78(side);
        if (unit == 0) {
            return -1;
        }
        int fighter = unit->info->id;
        unsigned char missed = 0;
        AimDraw draw;
        if (func_08891D84(phase->rules, &missed, &draw, 0, 0, 0) != 0) {
            return -2;
        }
        if (!missed) {
            int slot = func_089E99C0(side, fighter);
            func_089F84E0(phase->gauge);
            func_089F85B0(phase->gauge, fighter, slot);
            phase->state = 4;
        } else {
            phase->state = 7;
        }
        if (!missed && AimDraw::doubled(&draw) == 1) {
            func_08A62020(D_08A62280, 3, 0.0f);
        }
        func_08A10DB0(phase->timer);
        func_08A10F08(phase->timer);
        func_08A01CF4(phase->aim_cue);
        phase->time = 0.0f;
    }
    return 0;
}

}
