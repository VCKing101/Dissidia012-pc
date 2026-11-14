/* True when each of the two units is absent or has flag 2 set. */
struct MarkUnit {
    char pad_00[0x10];
    unsigned int flags;

    static bool marked(MarkUnit *u)
    {
        return (u->flags & 2) != 0;
    }
};

struct MarkPair {
    char pad_000[0x5FC];
    MarkUnit *units[2];
};

extern "C" {
int func_088E3428(MarkPair *pair)
{
    for (int i = 0; i < 2; i++) {
        if (pair->units[i] != 0 && !MarkUnit::marked(pair->units[i])) {
            return 0;
        }
    }
    return 1;
}
}
