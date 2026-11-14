typedef struct PhaseRunner {
    unsigned char active;
    char pad_01[0x3];
    int phase;
} PhaseRunner;

extern void func_089E75BC(PhaseRunner *self);
extern void func_089E7628(PhaseRunner *self);
extern void func_089E77B8(PhaseRunner *self);
extern void func_089E77DC(PhaseRunner *self);

int func_089E752C(PhaseRunner *self)
{
    if (!self->active) {
        return -1;
    }
    switch (self->phase) {
    case 0:
        func_089E75BC(self);
        break;
    case 1:
        func_089E7628(self);
        break;
    case 2:
        func_089E77B8(self);
        break;
    }
    func_089E77DC(self);
    return 0;
}
