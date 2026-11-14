extern "C" {
extern int func_089EA11C(void *);
extern void func_089EAC98(void *, int);
}

struct SlotPair84C { char data[0x84C]; };
struct SlotPairOwner {
    char pad_000[0x1C];
    SlotPair84C slots[2];
};

extern "C" {
int func_089EA324(SlotPairOwner *self)
{
    if (!func_089EA11C(self)) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        func_089EAC98(&self->slots[i], 0);
    }
    return 0;
}
}
