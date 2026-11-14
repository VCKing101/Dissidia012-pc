/* Links an actor's group with every other active fighter's group. */
struct Actor08833360 {
    char pad_000[0x468];
    void *group;
    char pad_46C[0x542 - 0x46C];
    unsigned char kind;

    static bool is_kind(Actor08833360 *a, int kind) { return a->kind == kind; }
};

extern "C" {

Actor08833360 *func_08841F64(Actor08833360 *prev);
int func_08837990(Actor08833360 *actor);
int func_088375D4(Actor08833360 *actor);
void func_0883E904(void *a, void *b, int mode);

void func_08833360(Actor08833360 *self, int mode)
{
    Actor08833360 *a = 0;
    while ((a = func_08841F64(a)) != 0) {
        if (a != self && !func_08837990(a) && func_088375D4(a) && !Actor08833360::is_kind(a, 3)) {
            func_0883E904(self->group, a->group, mode);
        }
    }
}

}
