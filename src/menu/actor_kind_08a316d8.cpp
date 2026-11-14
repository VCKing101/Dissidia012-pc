extern "C" {
struct Actor08A316D8 { char pad[6]; signed char kind; };
extern Actor08A316D8 *func_088470B8(int id);
}

/* True when the actor with this id exists and has kind 0x16. */
extern "C" int func_08A316D8(int self, int id)
{
    Actor08A316D8 *actor = func_088470B8(id);
    if (actor != 0 && actor->kind == 0x16) {
        return 1;
    } else {
        return 0;
    }
}
