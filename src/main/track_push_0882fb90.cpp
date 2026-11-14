/* Queues (id, a, b) on the player's ring while a track is loaded. */
struct Player0882FB90 {
    char pad_00[0x48];
    void *track;

    static bool loaded(Player0882FB90 *p) { return p->track != 0; }
};

extern "C" {

void func_0882F49C(Player0882FB90 *p, int id, float a, float b);

void func_0882FB90(Player0882FB90 *p, int id, float a, float b)
{
    if (!Player0882FB90::loaded(p)) {
        return;
    }
    func_0882F49C(p, id, a, b);
}

}
