/* Returns the entry's flag byte only while the global state has no pending id (-1), else 0. */
struct State38 {
    char pad[0x4D0];
    int pending;
    static unsigned char Pending(State38 *s) { return s->pending + 1 != 0 ? 1 : 0; }
};
struct Ent38 { unsigned char active; char pad[7]; unsigned char flag; };
extern "C" State38 *D_089D6AC8;
extern "C" unsigned char func_08AA4528(Ent38 *e)
{
    if (e->active == 0) {
        return 0;
    }
    if (State38::Pending(D_089D6AC8) == 1) {
        return 0;
    } else {
        return e->flag;
    }
}
