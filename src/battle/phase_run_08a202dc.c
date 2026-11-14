typedef struct PhaseRun202DC {
    unsigned char active;
} PhaseRun202DC;

extern void func_08A2045C(PhaseRun202DC *self);
extern int func_08A204E8(PhaseRun202DC *self);
extern void func_08A2052C(PhaseRun202DC *self);

int func_08A202DC(PhaseRun202DC *self)
{
    if (!self->active) {
        return -1;
    }
    func_08A2045C(self);
    func_08A204E8(self);
    func_08A2052C(self);
    return 0;
}
