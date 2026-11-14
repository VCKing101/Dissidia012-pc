/* Per-frame update; outside phase 3 also resets the battle state and lights. */
struct PhaseOwner {
    char pad_0000[0x1340];
    char light[0x13EC - 0x1340];
    char extra[4];
    signed char extra_on;
    char pad_13F1[0x1860 - 0x13F1];
    int phase;
    char pad_1864[0x1874 - 0x1864];
    char slot[0x1AF0 - 0x1874];
    char timer[0x10];

    static bool isSet(int value)
    {
        return value != 0;
    }
};

extern "C" {
extern void *D_089D6AE8;
extern int func_088D7574(void *slot);
extern void func_088CE7F4(void *timer);
extern void func_088E9704(PhaseOwner *owner, int mode);
extern void func_088A28E8(int a, int b);
extern void func_088965F4(void *p, int arg);
extern void func_088A3F2C(int arg);
extern void func_088E6940(void *light);
extern void func_088EA504(void *extra);

void func_088E9670(PhaseOwner *owner)
{
    func_088D7574(owner->slot);
    func_088CE7F4(owner->timer);
    if (owner->phase != 3) {
        func_088E9704(owner, 1);
        func_088A28E8(0, 0);
        func_088965F4(D_089D6AE8, 0);
        func_088A3F2C(0);
        func_088E6940(owner->light);
        if (PhaseOwner::isSet(owner->extra_on)) {
            func_088EA504(owner->extra);
        }
    }
}
}
