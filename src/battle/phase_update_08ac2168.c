typedef struct PhaseOwner {
    unsigned char active;
} PhaseOwner;

extern void func_08AC2508(PhaseOwner *self, float dt);
extern void func_08AC34A4(PhaseOwner *self, float dt);
extern void func_08AC257C(PhaseOwner *self, float dt);
extern void func_08AC2710(PhaseOwner *self);
extern void func_08AC27DC(PhaseOwner *self);

int func_08AC2168(PhaseOwner *self, float dt)
{
    if (!self->active) {
        return -1;
    }
    func_08AC2508(self, dt);
    func_08AC34A4(self, dt);
    func_08AC257C(self, dt);
    func_08AC2710(self);
    func_08AC27DC(self);
    return 0;
}
