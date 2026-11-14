/* Link-set lookups: entries of 0x1C bytes from offset 0xC, count at 0x4. */
typedef struct LinkEntry {
    char data[0x1C];
} LinkEntry;

typedef struct LinkSet {
    int field_00;
    short count;
    short field_06;
    int field_08;
    LinkEntry entries[1];
} LinkSet;

extern int func_08812D48(LinkEntry *entry, LinkEntry *key);
LinkEntry *func_0883EA64(LinkSet *set, LinkEntry *key);

LinkEntry *func_0883E9E4(LinkSet *set, LinkSet *other)
{
    int i;
    LinkEntry *found;

    for (i = 0; i < other->count; i++) {
        found = func_0883EA64(set, &other->entries[i]);
        if (found != 0) {
            return found;
        }
    }
    return 0;
}

LinkEntry *func_0883EA64(LinkSet *set, LinkEntry *key)
{
    int i;

    for (i = 0; i < set->count; i++) {
        if (func_08812D48(&set->entries[i], key) != 0) {
            return &set->entries[i];
        }
    }
    return 0;
}
