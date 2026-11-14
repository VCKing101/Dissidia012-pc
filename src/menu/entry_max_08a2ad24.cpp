struct Entry2AD24 {
    char pad_00;
    unsigned char value;
};

extern "C" {
extern int D_089D712C;
extern Entry2AD24 *func_088CB164(int table, int index);
}

extern "C" int func_08A2AD24(int self, int index)
{
    int best;
    int skip;
    int i;
    int n;
    best = 0;
    n = index;
    if (index > 0) {
        return func_088CB164(D_089D712C + 0xF58, n)->value;
    }
    i = 1;
    skip = 0x17;
    for (; i < 0x21; i++) {
        if (i != skip) {
            int value = func_088CB164(D_089D712C + 0xF58, i)->value;
            if (best < value) {
                best = value;
            }
        }
    }
    return best;
}
