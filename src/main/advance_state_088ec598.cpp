/* In state 1, when the owner's flag word has bit 0x20, fires the stored callback with its argument and moves to state 2. */
struct Owner { char pad[0x20]; unsigned short flags; };
struct Thing {
    char pad[4]; Owner *owner; int state; void (*cb)(int); int arg;
    static bool has(int f) { return (f & 0x20) != 0; }
};
extern "C" void func_088EC598(Thing *self)
{
    switch (self->state) {
    case 0:
        break;
    case 1:
        if (Thing::has(self->owner->flags) == 1) {
            if (self->cb != 0) {
                self->cb(self->arg);
            }
            self->state = 2;
        }
        break;
    case 2:
        break;
    }
}
