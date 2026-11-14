/* Advances a pair timer unless the battle is paired or in mode 4, and fires
 * it when it reaches its limit. */
struct BattleRules {
    unsigned char pad_000[0x278];
    int pair_mode;

    static bool paired(BattleRules *rules) { return rules->pair_mode != 0; }
};

struct BattleSession {
    unsigned char pad_000[0x48];
    int mode;

    static bool is(BattleSession *session, int mode) { return session->mode == mode; }
};

struct FrameClock {
    unsigned char pad_00[0x18];
    float frame_scale;
};

struct PairTimer {
    unsigned char pad_00[0xC];
    int fired;
    float time;
    unsigned char pad_14[0x38];
    float limit;
};

extern "C" {

extern BattleRules *D_08A405E0;
extern BattleSession *D_089D6A10;
extern FrameClock *D_089D69E0;
extern void func_089E8A00(PairTimer *timer);

int func_089E85FC(PairTimer *timer)
{
    BattleRules *rules = D_08A405E0;
    float time = timer->time;
    if (!BattleRules::paired(rules) && !BattleSession::is(D_089D6A10, 4)) {
        time += D_089D69E0->frame_scale;
        timer->time = time;
    }
    if (time >= timer->limit) {
        func_089E8A00(timer);
        timer->fired = 1;
        timer->time = 0.0f;
    }
    return 0;
}

}
