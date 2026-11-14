typedef struct PhaseRun26484 {
    unsigned char active;
    char pad_001[0x1B];
    char field_1C[0x38];
    char field_54[0x88];
    char field_DC[0x4];
} PhaseRun26484;

extern void func_08A265FC(PhaseRun26484 *self);
extern void func_089F83AC(void *sub);
extern void func_08A24ED4(void *sub);
extern void func_08A08334(void *sub);

int func_08A26484(PhaseRun26484 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A265FC(self);
    func_089F83AC(self->field_1C);
    func_08A24ED4(self->field_54);
    func_08A08334(self->field_DC);
    return 0;
}
