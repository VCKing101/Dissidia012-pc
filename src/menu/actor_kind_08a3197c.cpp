extern "C" {
struct Actor08A3197C { char pad[6]; signed char kind; };
extern Actor08A3197C *func_088470B8(int id);
}

/* True when the actor with this id exists and has kind 0x7. */
extern "C" int func_08A3197C(int self, int id)
{
    Actor08A3197C *actor = func_088470B8(id);
    if (actor != 0 && actor->kind == 0x7) {
        return 1;
    } else {
        return 0;
    }
}
