struct GuardState {
    unsigned char open;
    unsigned char held;
    char pad_02[0x2];
    float time;
};

struct GuardFighter {
    char pad_000[0x130];
    char effect[0x254];
    unsigned int flags;
    char pad_388[0x20];
    GuardState *guard;

    static unsigned char blocked(GuardFighter *self) { return (self->flags & 0x200) != 0; }
};

extern "C" {

extern void func_0882FA10(float x, float y, void *effect, int id, int flags);

/* Opens the guard state (closed while flag 0x200 is set) and plays its effect. */
void func_08A2E3E4(void *owner, GuardFighter *self)
{
    GuardState *guard = self->guard;
    float zero = 0.0f;
    guard->open = !GuardFighter::blocked(self);
    self->flags |= 0x20000;
    func_0882FA10(zero, zero, self->effect, 0x4E34, 0);
    guard->held = 0;
    guard->time = zero;
    self->flags |= 0x200000;
}

}
