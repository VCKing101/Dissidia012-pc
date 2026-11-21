struct Entry0884F7C4 {
    unsigned char unk_00[0x12];
    short id;                          /* +0x12 0 = unused */
    unsigned char unk_14[7];
    unsigned char enabled;             /* +0x1B */
    unsigned char unk_1C[4];
};

extern "C" {
int func_0884EDC0(Entry0884F7C4 *e, void *b);
void func_0884EEE0(Entry0884F7C4 *e, void *a, void *c);

/* Hand `a` and `c` to the first used, enabled entry that accepts `b`. */
void func_0884F7C4(Entry0884F7C4 *entries, void *b, void *a, void *c)
{
    int i = 0;

    for (Entry0884F7C4 *e = &entries[i]; i < 16; e++, i++) {
        if (!(unsigned char)(e->id == 0)) {
            if (e->enabled != 0 && func_0884EDC0(e, b) != 0) {
                func_0884EEE0(e, a, c);
                return;
            }
        }
    }
}
}
