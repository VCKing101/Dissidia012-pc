typedef struct StreamSet {
    unsigned char active;
    char pad_01[0x3];
    unsigned int phase;
} StreamSet;

extern "C" {

extern int func_08B04F80(StreamSet *self);

bool func_08B04FCC(StreamSet *self)
{
    if (func_08B04F80(self) == 0) {
        return false;
    }
    return self->phase < 4;
}

}
