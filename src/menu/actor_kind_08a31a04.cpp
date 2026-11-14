extern "C" {
struct Actor08A31A04 { char pad[6]; signed char kind; };
extern Actor08A31A04 *func_088470B8(int id);
}

/* True when the actor with this id exists and has kind 0x17. */
extern "C" int func_08A31A04(int self, int id)
{
    Actor08A31A04 *actor = func_088470B8(id);
    if (actor != 0 && actor->kind == 0x17) {
        return 1;
    } else {
        return 0;
    }
}
