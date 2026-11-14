/* Stops the current track and restarts the player at the start with no fade. */
struct Player0882F9A8 {
    char pad_00[0x48];
    void *track;
    char pad_4C[4];
    short current;

    static bool loaded(Player0882F9A8 *p) { return p->track != 0; }
};

extern "C" {

void func_0882FBF0(Player0882F9A8 *p);
void func_0882EF10(Player0882F9A8 *p, int arg, float a, float b);

void func_0882F9A8(Player0882F9A8 *p, int arg)
{
    if (!Player0882F9A8::loaded(p)) {
        return;
    }
    func_0882FBF0(p);
    p->current = -1;
    func_0882EF10(p, arg, 0.0f, 0.0f);
}

}
