struct PairTarget {
    unsigned char active;
    char pad_01[0x10];
    unsigned char locked;
    char pad_12[0x2];
    unsigned int target;
};

extern "C" {

unsigned char func_08A0777C(PairTarget *self, unsigned int index)
{
    if (!self->active) {
        return 0;
    }
    if (index >= 2) {
        return 0;
    }
    int result = 0;
    if (self->locked == 1 && index != self->target) {
        result = 1;
    }
    return result;
}

}
