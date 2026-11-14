extern "C" {
struct Actor08A31868 { char pad[6]; signed char kind; };
extern Actor08A31868 *func_088470B8(int id);
}

/* True when the actor with this id exists and has kind 0x7. */
extern "C" int func_08A31868(int self, int id)
{
    Actor08A31868 *actor = func_088470B8(id);
    if (actor != 0 && actor->kind == 0x7) {
        return 0;
    } else {
        return 1;
    }
}
