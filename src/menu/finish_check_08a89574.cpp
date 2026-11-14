/* Once the fade (or the wait flag) is done and both checks pass, closes the panel and sets state 0x19A. */
struct FinishState {
    char pad_000[0x41C];
    unsigned int field_41C;
    int field_420;
    char pad_424[0x4];
    int field_428;
};

struct FinishOwner {
    char pad_000[0x154];
    void *field_154;
    char pad_158[0x30C - 0x158];
    FinishState *field_30C;
    static bool hasFlag(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
};

extern "C" {
int func_08873EB4(void);
int func_08A8EE44(void *panel);
int func_08A89F7C(FinishOwner *self);
void func_08A8ED84(void *panel);

void func_08A89574(FinishOwner *self)
{
    if (self->field_30C->field_428 == 0) {
        if (func_08873EB4() == 0) {
            return;
        }
    } else if (!FinishOwner::hasFlag(self->field_30C->field_41C, 0x40000)) {
        return;
    }
    if (func_08A8EE44(self->field_154) == 0) {
        return;
    }
    if (func_08A89F7C(self) == 0) {
        return;
    }
    func_08A8ED84(self->field_154);
    self->field_30C->field_420 = 0x19A;
}
}
