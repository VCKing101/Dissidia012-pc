/* Runs the controller's three parts (step 0xc) when `on` is set; -1
   otherwise. Calls func_08A9B8A8 when the part at +0x158 reports 0.
   Retail keeps `self` in the frame (0($sp)) across the calls: a local object
   with a constructor (Kept) is never promoted by SNC -O2. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Control08A9ED0C {
    char unknown_000[0x158];
    void *part_158;
    void *part_15C;
    int unknown_160;
    void *part_164;
};

extern "C" {

void func_08A97060(void *part, int step);
void func_08A67AC0(void *part);
int func_08A65904(void *part);
void func_08A9B8A8(Control08A9ED0C *self);

int func_08A9ED0C(Control08A9ED0C *self, int on)
{
    if (on == 0) {
        return -1;
    }
    Kept<Control08A9ED0C *> saved(self);
    func_08A97060(self->part_164, 0xC);
    func_08A67AC0(saved.value->part_15C);
    int done = func_08A65904(saved.value->part_158);
    self = saved.value;
    if (done == 0) {
        func_08A9B8A8(self);
    }
    return 0;
}

}
