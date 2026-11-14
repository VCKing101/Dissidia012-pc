extern "C" {
struct Actor08A31938 { char pad[6]; signed char kind; };
extern Actor08A31938 *func_088470B8(int id);
}

/* True when the actor with this id exists and has kind 0x5. */
extern "C" int func_08A31938(int self, int id)
{
    Actor08A31938 *actor = func_088470B8(id);
    if (actor != 0 && actor->kind == 0x5) {
        return 1;
    } else {
        return 0;
    }
}
