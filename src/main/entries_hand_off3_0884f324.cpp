struct Entry0884F324 {
    unsigned char unk_00[0x12];
    short id;                          /* +0x12 0 = unused */
    unsigned char unk_14[7];
    unsigned char enabled;             /* +0x1B */
    unsigned char unk_1C[4];
};

extern "C" {
int func_0884EDC0(Entry0884F324 *e, void *key);
void func_0884ED1C(Entry0884F324 *e, void *a, void *b, void *c);

/* Hand a, b, c to the first used, enabled entry that accepts `key`. */
void func_0884F324(Entry0884F324 *entries, void *a, void *b, void *c, void *key)
{
    int i = 0;

    for (Entry0884F324 *e = &entries[i]; i < 16; e++, i++) {
        if (!(unsigned char)(e->id == 0)) {
            if (e->enabled != 0 && func_0884EDC0(e, key) != 0) {
                func_0884ED1C(e, a, b, c);
                return;
            }
        }
    }
}
}
