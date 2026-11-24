/* Shows the battle HUD for the mode's kind, stage and layout, animated in. */
typedef struct ModeCtl {
    char pad_000[0x4C];
    int kind;
    char pad_050[4];
    int stage;
    char pad_058[0x740 - 0x58];
    int layout;
} ModeCtl;

extern ModeCtl *D_089D6A10;
extern int D_08AA3F44;

extern void func_08A97FA4(int hud, int kind, int stage, int layout, int flag);

void func_0888AB28(void)
{
    ModeCtl *mode = D_089D6A10;
    int kind = mode->kind;
    int stage = mode->stage;
    int layout = mode->layout;
    func_08A97FA4(D_08AA3F44, kind, stage, layout, 1);
}
