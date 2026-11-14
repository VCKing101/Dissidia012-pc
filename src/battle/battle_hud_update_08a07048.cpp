/* Per-frame update of the battle HUD and its per-side displays. */
struct HudSideBanner {
    unsigned char data[0x34];
};

struct HudSideTimer {
    unsigned char data[0x2C];
};

struct HudSideEffects {
    unsigned char data[0xB0];
};

struct BattleHud {
    unsigned char open;
    unsigned char pad_01[0x10];
    unsigned char flashing;
    unsigned char pad_12[2];
    int flash_side;
    unsigned char pad_18[0x10];
    unsigned char clock[0xC];
    unsigned char counter[0x14];
    unsigned char pop[0x11C];
    HudSideBanner banners[2];
    HudSideTimer timers[2];
    HudSideEffects effects[2];
};

extern "C" {

extern int func_08A156C0(void *clock);
extern void func_08A079C4(BattleHud *hud);
extern void func_08A07A18(BattleHud *hud);
extern void func_08A07A74(BattleHud *hud);
extern void func_08A1564C(void *clock);
extern void func_08A113D8(void *counter);
extern int func_089F0FE8(void *pop);
extern void func_08A07C6C(BattleHud *hud);
extern void func_08A06370(HudSideBanner *banner);
extern void func_08A10C88(HudSideTimer *timer);
extern int func_08A03A24(HudSideEffects *effects);
extern void func_08A07CFC(BattleHud *hud);

int func_08A07048(BattleHud *hud)
{
    if (!hud->open) {
        return -1;
    }
    if (func_08A156C0(hud->clock) == 1) {
        hud->flashing = 0;
        hud->flash_side = -1;
    }
    func_08A079C4(hud);
    func_08A07A18(hud);
    func_08A07A74(hud);
    func_08A1564C(hud->clock);
    func_08A113D8(hud->counter);
    func_089F0FE8(hud->pop);
    func_08A07C6C(hud);
    for (int i = 0; i < 2; i++) {
        func_08A06370(&hud->banners[i]);
        func_08A10C88(&hud->timers[i]);
        func_08A03A24(&hud->effects[i]);
    }
    func_08A07CFC(hud);
    return 0;
}

}
