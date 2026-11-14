typedef struct MatchRecord {
    char pad_00[0x13];
    signed char group;
    signed char slot;
    char pad_15[0x3];
} MatchRecord;

struct MatchOps {
    static unsigned char same(MatchRecord *a, MatchRecord *b)
    {
        int result = 0;
        if (a->group == b->group && a->slot == b->slot) {
            result = 1;
        }
        return result;
    }
};

extern "C" {

int func_08B00AB8(MatchRecord **out, int max, MatchRecord *first, MatchRecord *last, MatchRecord *key)
{
    int count = 0;
    for (; first != last; first++) {
        if (MatchOps::same(first, key) == 1) {
            if (count < max) {
                *out = first;
                count++;
                out++;
            } else {
                break;
            }
        }
    }
    return count;
}

}
