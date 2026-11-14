/* Updates the HUD parts and sets the visible flag from virtual 0. */
struct HudBase08872F08 {
    char pad_000[0x34];
    virtual int visible(unsigned long long mask);
};

struct Hud08872F08 : HudBase08872F08 {
    char pad_038[0x22C];
    unsigned int flags;
};

extern "C" {

extern Hud08872F08 *D_089D6008;
extern unsigned long long D_089C9AC0;

void func_08872CF0(Hud08872F08 *hud);
void func_08872D88(Hud08872F08 *hud);
void func_08872DEC(Hud08872F08 *hud);

void func_08872F08(void)
{
    Hud08872F08 *hud = D_089D6008;
    func_08872CF0(hud);
    func_08872D88(hud);
    func_08872DEC(hud);
    if (hud->visible(D_089C9AC0)) {
        unsigned int flags = hud->flags | 4;
        hud->flags = flags;
    } else {
        unsigned int flags = hud->flags & ~4;
        hud->flags = flags;
    }
}

}
