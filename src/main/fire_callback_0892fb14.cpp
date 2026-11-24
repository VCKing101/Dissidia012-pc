/* When the owner's flag word has bit 0x20, runs the cleanup step, clears the state and fires the stored callback. */
struct Owner { char pad[0x20]; unsigned short flags; };
struct Thing {
    char pad[0x28]; int state; char pad2[8]; Owner *owner; char pad3[0x118]; void (*cb)(void);
    static bool has(int f) { return (f & 0x20) != 0; }
};
extern "C" void func_0892F6DC(Thing *self);
extern "C" void func_0892FB14(Thing *self)
{
    if (!Thing::has(self->owner->flags)) {
        return;
    }
    func_0892F6DC(self);
    self->state = 0;
    if (!self->cb) {
        return;
    }
    self->cb();
}
