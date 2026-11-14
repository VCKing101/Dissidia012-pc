typedef struct PhaseRun257E8 {
    unsigned char active;
    char pad_001[0x13];
    char field_14[0x38];
    char field_4C[0x4];
} PhaseRun257E8;

extern void func_08A258DC(PhaseRun257E8 *self);
extern void func_089F83AC(void *sub);
extern void func_08A25D2C(void *sub);

int func_08A257E8(PhaseRun257E8 *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A258DC(self);
    func_089F83AC(self->field_14);
    func_08A25D2C(self->field_4C);
    return 0;
}
