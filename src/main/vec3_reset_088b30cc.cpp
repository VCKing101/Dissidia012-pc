/* Initialises the vectors and clears the three counters at 0x24. */
struct Vec088B30CC {
    int x;
    int y;
    int z;
};

struct Triple088B30CC {
    Vec088B30CC vecs[3];
    int counts[3];
};

extern "C" {

void func_088B2BCC(Triple088B30CC *self);

void func_088B30CC(Triple088B30CC *self)
{
    func_088B2BCC(self);
    for (int i = 0; i < 2; i++) {
        self->counts[i] = 0;
    }
    for (int i = 2; i < 3; i++) {
        self->counts[i] = 0;
    }
}

}
