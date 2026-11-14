/* Initialises the two 12-byte vectors and the third one at 0x18. */
struct Vec088B2BCC {
    int x;
    int y;
    int z;
};

struct Triple088B2BCC {
    Vec088B2BCC pair[2];
    Vec088B2BCC third;
};

extern "C" {

void func_088B2B14(Vec088B2BCC *v);

void func_088B2BCC(Triple088B2BCC *self)
{
    for (int i = 0; i < 2; i++) {
        func_088B2B14(&self->pair[i]);
    }
    func_088B2B14(&self->third);
}

}
