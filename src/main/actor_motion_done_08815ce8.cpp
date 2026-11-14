/* Whether an actor's special motion has finished. */
struct Actor08815CE8 {
    char pad_000[0x178];
    void *special;
    char pad_17C[4];
    short motion;
    char pad_182[2];
    unsigned int motion_flags;
    char pad_188[0x560 - 0x188];
    unsigned int flags;
    char pad_564[0x570 - 0x564];
    char player[1];

    static bool has_special(Actor08815CE8 *a) { return a->special != 0; }
};

extern "C" {

int func_08822D30(char *player);
bool func_08822D58(char *player);

bool func_08815CE8(Actor08815CE8 *self)
{
    if (!(self->flags & 1)) {
        return false;
    }
    char *player = self->player;
    if (func_08822D30(player) == 1) {
        return func_08822D58(player);
    }
    if (Actor08815CE8::has_special(self) != true) {
        return true;
    }
    if (self->motion == 0x3E9) {
        return false;
    }
    return (self->motion_flags & 2) != 0;
}

}
