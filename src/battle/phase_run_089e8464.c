typedef struct PhaseRunner {
    char pad_00[0x8];
    unsigned char active;
    char pad_09[0x3];
    int phase;
} PhaseRunner;

extern void func_089E85FC(PhaseRunner *self);
extern void func_089E8698(PhaseRunner *self);
extern void func_089E87BC(PhaseRunner *self);

int func_089E8464(PhaseRunner *self)
{
    if (!self->active) {
        return -1;
    }
    if (self->phase == 0) {
        func_089E85FC(self);
    }
    if (self->phase == 1) {
        func_089E8698(self);
    }
    if (self->phase == 2) {
        func_089E87BC(self);
    }
    return 0;
}
