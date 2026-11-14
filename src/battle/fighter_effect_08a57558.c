typedef struct Fighter57558 {
    char pad_000[0x130];
    char effects[0x4];
} Fighter57558;

extern void func_08834C10(Fighter57558 *fighter, int state, int arg);
extern void func_0882FA10(void *effects, int id, int arg, float a, float b);

void func_08A57558(Fighter57558 *fighter, unsigned char strong)
{
    func_08834C10(fighter, 9, 0);
    if (strong) {
        func_0882FA10(fighter->effects, 0x320, 0, 2.0f, 24.0f);
    } else {
        func_0882FA10(fighter->effects, 0x320, 0, 0.0f, 0.0f);
    }
}
