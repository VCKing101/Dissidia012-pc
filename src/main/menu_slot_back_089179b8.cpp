struct Cur089179B8 { char pad[0x50]; int slot; };
struct Bits089179B8 { char pad[0x260]; unsigned long long bits; };
extern Cur089179B8 *D_089D83D8;
extern int D_089D83DC;
extern int D_089D85E8;
extern int D_089E75C4;
extern int D_08A8B7A8;
extern unsigned long long D_089CE380;
extern unsigned long long D_089CE388;
extern "C" Bits089179B8 *func_0891705C(int);
extern "C" void func_08917E04(int, int);
extern "C" void func_08917E78(int);
extern "C" void func_08933C00(int);
extern "C" void func_089E0BDC(int, int);
extern "C" void func_08A88EDC(int, int, int);
struct Hlp089179B8 { static unsigned char any(Bits089179B8 *b) { return (b->bits & D_089CE380) != 0; } };
extern "C" void func_089179B8()
{
    Bits089179B8 *b = func_0891705C(-1);
    int slot = D_089D83D8->slot;
    if (slot >= 0) {
        func_08917E04(D_089D83DC, slot);
        func_08917E78(D_089D83DC);
    } else {
        func_08933C00(D_089D85E8);
        if (Hlp089179B8::any(b)) {
            *(unsigned long long *)((char *)b + 0x260) &= D_089CE388;
            int g = D_089E75C4;
            int zero = 0;
            func_089E0BDC(g, zero);
        } else {
            func_08A88EDC(D_08A8B7A8, 1, -1);
        }
    }
}
