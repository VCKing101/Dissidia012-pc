/* Initialises the vectors and clears the two counters at 0x24. */
struct Vec088B3174 {
    int x;
    int y;
    int z;
};

struct Pair088B3174 {
    Vec088B3174 vecs[3];
    int counts[2];
};

extern "C" {

void func_088B2BCC(Pair088B3174 *self);

void func_088B3174(Pair088B3174 *self)
{
    func_088B2BCC(self);
    for (int i = 0; i < 2; i++) {
        self->counts[i] = 0;
    }
}

}
