extern "C" {
struct Actor08A319C0 { char pad[6]; signed char kind; };
extern Actor08A319C0 *func_088470B8(int id);
}

/* True when the actor with this id exists and has kind 0x12. */
extern "C" int func_08A319C0(int self, int id)
{
    Actor08A319C0 *actor = func_088470B8(id);
    if (actor != 0 && actor->kind == 0x12) {
        return 1;
    } else {
        return 0;
    }
}
