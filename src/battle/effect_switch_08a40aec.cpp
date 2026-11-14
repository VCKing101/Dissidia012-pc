struct EffectSet { char pad_00[0x28]; int handle; };
struct Scene { char pad_000[0x34]; EffectSet effects; };
struct Owner { char pad_000[0x390]; Scene *scene; char pad_394[0x134]; char sound_4C8[4]; };
struct Info { char pad_00[0x3C]; int effect; };
struct Ctx { Owner *owner; char pad_04[0x10]; Info *info; };
struct Effects {
    static int handleOf(EffectSet *set) { return ((int *)set)[10]; }
};
extern "C" {
extern void func_088A6C70(int effect);
extern void func_088A9DE8(int handle, int id, void *sound, int a, int b);

/* Stops the current effect and starts the one for this kind, if it has one. */
void func_08A40AEC(Ctx *self, int kind)
{
    int ids[5] = { -1, 0x2E, 0x2F, -1, 0x30 };
    if (self->info->effect != 0) {
        func_088A6C70(self->info->effect);
    }
    self->info->effect = 0;
    if (ids[kind] == -1) {
        return;
    }
    func_088A9DE8(Effects::handleOf(&self->owner->scene->effects), ids[kind], self->owner->sound_4C8, 0, 0);
}
}
