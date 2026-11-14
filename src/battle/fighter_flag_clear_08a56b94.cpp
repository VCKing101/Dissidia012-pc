struct HitInfo56B94 {
    unsigned char active;
};

struct Fighter56B94 {
    char pad_000[0x188];
    unsigned int state_flags;
    char pad_18C[0x21C];
    HitInfo56B94 *info;

    static bool hasFlag10(Fighter56B94 *fighter) { return (fighter->state_flags & 0x10) != 0; }
};

extern "C" {

extern void func_08A5B4E0(void *context, Fighter56B94 *fighter);

void func_08A56B94(void *context, Fighter56B94 *self)
{
    self->info->active = 0;
    if (Fighter56B94::hasFlag10(self)) {
        self->state_flags &= ~0x10;
    }
    func_08A5B4E0(context, self);
}

}
