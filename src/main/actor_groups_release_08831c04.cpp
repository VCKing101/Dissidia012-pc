/* Releases every hit group of an actor. */
struct Actor08831C04 {
    char pad_000[0x468];
    void *groups[10];
};

extern "C" {

void func_0883E75C(void *group);

void func_08831C04(Actor08831C04 *self)
{
    if (self->groups[0] != 0) {
        func_0883E75C(self->groups[0]);
    }
    if (self->groups[1] != 0) {
        func_0883E75C(self->groups[1]);
    }
    if (self->groups[2] != 0) {
        func_0883E75C(self->groups[2]);
    }
    if (self->groups[3] != 0) {
        func_0883E75C(self->groups[3]);
    }
    if (self->groups[4] != 0) {
        func_0883E75C(self->groups[4]);
    }
    if (self->groups[5] != 0) {
        func_0883E75C(self->groups[5]);
    }
    if (self->groups[6] != 0) {
        func_0883E75C(self->groups[6]);
    }
    if (self->groups[7] != 0) {
        func_0883E75C(self->groups[7]);
    }
    if (self->groups[8] != 0) {
        func_0883E75C(self->groups[8]);
    }
    if (self->groups[9] != 0) {
        func_0883E75C(self->groups[9]);
    }
}

}
