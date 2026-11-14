typedef struct HudState {
    char pad000[0x198];
    int value;              /* 0x198 */
    unsigned char active;   /* 0x19C */
    unsigned char field_19D;
    unsigned char field_19E;
    unsigned char field_19F;
} HudState;

int func_08B23DD0(HudState *hud)
{
    if (hud->active == 0) {
        return -1;
    }
    hud->active = 0;
    hud->field_19E = 0;
    hud->value = 0;
    return 0;
}
