/* Polls input, then moves to phase 2 once both slots are filled. */
struct PhaseOwner {
    char pad_0000[0x90];
    int first;
    int field_94;
    int second;
    char pad_009C[0x1860 - 0x9C];
    int phase;

    static bool isSet(int value)
    {
        return value != 0;
    }
};

extern "C" {
extern void func_08883F78(void);
extern void func_08883F64(void);

void func_088E9254(PhaseOwner *owner)
{
    func_08883F78();
    func_08883F64();
    if (PhaseOwner::isSet(owner->second) && PhaseOwner::isSet(owner->first)) {
        owner->phase = 2;
    }
}
}
