/* Calls func_0887B908 for each present unit without flag 2. */
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
extern void func_0887B908(MarkUnit *u);

void func_088E347C(MarkPair *pair)
{
    for (int i = 0; i < 2; i++) {
        MarkUnit *u = pair->units[i];

        if (u != 0 && !MarkUnit::marked(u)) {
            func_0887B908(u);
        }
    }
}
}
