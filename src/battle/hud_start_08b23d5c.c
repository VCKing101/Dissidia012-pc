typedef struct HudState {
    char pad000[0x198];
    int value;              /* 0x198 */
    unsigned char active;   /* 0x19C */
    unsigned char field_19D;
    unsigned char field_19E;
    unsigned char field_19F;
} HudState;

extern unsigned char D_089D6039;
extern void func_08B23060(HudState *hud);

int func_08B23D5C(HudState *hud, int value)
{
    if (value < 0) {
        return -1;
    }
    if (hud->active == 1) {
        return -2;
    }
    func_08B23060(hud);
    hud->active = 1;
    hud->field_19F = 0;
    hud->value = value;
    D_089D6039 = 1;
    return 0;
}
