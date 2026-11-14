/* Starts a 20-frame fade with the phase callback and moves to phase 4. */
typedef struct PhaseOwner {
    char pad_0000[0x1860];
    int phase;
} PhaseOwner;

extern void func_088E9B68(void);
extern void func_08873D34(float frames, int layer, void (*done)(void), int arg, int mode, int kind);
extern void func_08873DEC(int layer, int kind);

void func_088E92D4(PhaseOwner *owner)
{
    func_08873D34(20.0f, 3, func_088E9B68, 0, 1, 2);
    func_08873DEC(3, 2);
    owner->phase = 4;
}
