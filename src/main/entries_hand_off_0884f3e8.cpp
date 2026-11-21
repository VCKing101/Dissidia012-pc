struct Entry0884F3E8 {
    unsigned char unk_00[0x12];
    short id;                          /* +0x12 0 = unused */
    unsigned char unk_14[7];
    unsigned char enabled;             /* +0x1B */
    unsigned char unk_1C[4];
};

extern "C" {
int func_0884EDC0(Entry0884F3E8 *e, void *b);
void func_0884ED94(Entry0884F3E8 *e, void *a);

/* Hand `a` to the first used, enabled entry that accepts `b`. */
void func_0884F3E8(Entry0884F3E8 *entries, void *a, void *b)
{
    int i = 0;

    for (Entry0884F3E8 *e = &entries[i]; i < 16; e++, i++) {
        if (!(unsigned char)(e->id == 0)) {
            if (e->enabled != 0 && func_0884EDC0(e, b) != 0) {
                func_0884ED94(e, a);
                return;
            }
        }
    }
}
}
