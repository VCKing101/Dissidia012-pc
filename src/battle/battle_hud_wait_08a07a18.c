/* Turn HUD state 2: counts down the turn delay, then ends the turn and
 * moves to state 3. */
typedef struct TurnHudState {
    unsigned char open;
    unsigned char pad_01[3];
    int state;
    int delay;
} TurnHudState;

extern void *D_089D6DC8;
extern void func_088A9684(void *battle);

int func_08A07A18(TurnHudState *hud)
{
    if (hud->state == 2) {
        if (hud->delay > 0) {
            hud->delay--;
        } else {
            func_088A9684(D_089D6DC8);
            hud->state = 3;
        }
    }
    return 0;
}
