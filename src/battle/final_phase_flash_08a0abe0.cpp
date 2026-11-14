/* After the flash delay, flashes the turn marker at the side's screen
 * position and moves to state 2. */
struct Vec2 {
    float x;
    float y;
};

struct BannerFlags {
    unsigned char data[0x18];

    static unsigned char ready(BannerFlags *flags) { return (unsigned char)(flags->data[0x14] & 1); }
    static bool muted(BannerFlags *flags) { return (flags->data[0x14] & 0x20) != 0; }
};

struct FinalPhaseData {
    unsigned char pad_00[8];
    int state;
    unsigned char pad_0c[4];
    float time;
    unsigned char pad_14[0x2C];
    unsigned char flash[0x96C];
    int wins;
    int losses;
    unsigned char pad_9b4[0xC];
    BannerFlags *fighter;
};

extern "C" {

extern float D_08A0AEE0;
extern float D_08A0AEE8;
extern float D_08A0AEF0;
extern unsigned int func_08A22CA8(int turn);
extern void *func_08A22C78(int turn);
extern void func_08B33914(void *fighter);
extern void func_089F6560(void *flash, float x, float y, float scale, unsigned int color);

int func_08A0ABE0(FinalPhaseData *phase, float step)
{
    Vec2 pos[2];
    pos[0].x = 158.0f;
    pos[0].y = 233.0f;
    Vec2 *right = &pos[1];
    float right_x = 322.0f;
    right->y = 233.0f;
    right->x = right_x;
    phase->time += step;
    if (phase->time >= D_08A0AEE8 - D_08A0AEE0) {
        int kept_turn[1];
        int turn = (phase->wins + phase->losses) % 2;
        kept_turn[0] = turn;
        unsigned int side = func_08A22CA8(turn);
        turn = kept_turn[0];
        if (side >= 2) {
            return -1;
        }
        void *fighter = func_08A22C78(turn);
        if (fighter == 0) {
            return -2;
        }
        func_08B33914(fighter);
        if (BannerFlags::ready(phase->fighter) == 1 && !BannerFlags::muted(phase->fighter)) {
            func_089F6560(phase->flash, pos[side].x, pos[side].y, D_08A0AEF0, 0xFF4040FF);
        }
        phase->state = 2;
        phase->time = 0.0f;
    }
    return 0;
}

}
